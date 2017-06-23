#include "unitmanager.h"
#include "gamemanager.h"
#define SCREEN_WIDTH 800
#define  SCREEN_HEIGHT 600

//객체 생성
Player hero;
Item hpitem, powitem , spitem;
Enemy enemy[ENEMY_NUM], item, enemy2[30];
Enemy_Bullet e_bullet[ENEMY_BULLET_NUM], e_bullet2[ENEMY_BULLET_NUM], e_bullet_missle[5], e_bullet_missle2, e_bullet_laser;
Boss n_boss, f_boss;
Bullet H_N_bullet[HERO_BULLET_NUM], hitbox,barrior;

int bosshp = 1000;
int playerkillcount = 0;
int playerHP = 3;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
		WS_EX_TOPMOST | WS_POPUP, 100, 100,800, 600,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);

	//게임 오브젝트 초기화 
	init_game();

	// enter the main loop:

	MSG msg;
	bgmopen.lpstrElementName = L"bgm-1.mp3";
	bgmopen.lpstrDeviceType = L"MPEGVideo";
	 
	efopen.lpstrElementName = L"shootsound.wav";
	efopen.lpstrDeviceType = L"MPEGVideo";

	mciSendCommand(bgmID, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&bgmopen);
	bgmID = bgmopen.wDeviceID;
	mciSendCommand(1, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&bgmplay);

	mciSendCommand(efID, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&efopen);
	efID = efopen.wDeviceID;
	while (TRUE)
	{
		bgpos--;

		if (bgpos < -800){
			bgpos = 0;
	}
			DWORD starting_point = GetTickCount();

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			do_game_logic();

			render_frame(hWnd);

			// check the 'escape' key
			if (KEY_DOWN(VK_ESCAPE))
				PostMessage(hWnd, WM_DESTROY, 0, 0);

			while ((GetTickCount() - starting_point) < 25);
		}
	
	// clean up DirectX and COM
	cleanD3D();
	return msg.wParam;
	
}

// 게임 오버 대화상자
/*
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			hero.HP = 3;
			//MessageCheck = false;
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			PostQuitMessage(0);
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

*/
	
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	 
	switch (message)
	{
	case WM_CREATE:
		hero.HP = 5;
		SetTimer(hWnd, 1, 1000, NULL);
		SendMessage(hWnd, WM_TIMER, 1, 0);
		SetTimer(hWnd, 2, 400, NULL); 
		SendMessage(hWnd, WM_TIMER, 2, 0);
		return 0;

		playerHit();
	case WM_DESTROY:
	{
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		PostQuitMessage(0);
		return 0;
	}break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// 이미지 등록
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;


	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite object

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"E_bullet.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_enemy_bullet);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"E_bullet2.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_enemy_bullet2);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"E_bullet3.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_enemy_bullet3_missle);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"E_bullet2.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_enemy_bullet4_missle2);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"E_Laser.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_enemy_bullet5_laser);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"background.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&bg_1);    // load to sprite
	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"background.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&bg_2);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"boss_1.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_boss);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"boss_2.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_fboss);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"Fire.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&fire);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"player1.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_hero);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"enemy2.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_enemy);    // load to sprite
	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"player_back.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_hero2);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"player1_down.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_hero2);    // load to sprite



	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"enemy.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&g_enemy2);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"p_bullet.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&H_N_bullet[0].g_pbullet);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"p_bullet2.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&H_N_bullet[0].g_pbullet2);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"barrior.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&H_N_bullet[0].g_pbarrior);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"p_bullet3.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&H_N_bullet[0].g_pbullet3);    // load to sprite

	
	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"HP.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&hp1);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"HP_nope.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&hp2);    // load to sprite
	
	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"powitem.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&item_pow);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"hpitem.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&item_hp);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"spitem.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&item_sp);    // load to sprite

	return;
} // 이미지 삽입 준비

  // 초기화
void init_game(void)
{
	//객체 초기화 
	hero.init(0, 300);

	for (int i = 0; i < 5; i++)
	{
		enemy2[i].HP = 50;
	}

	Boss_Check = false;
	n_boss.hide();
	n_boss.HP = BOSS_HP;
	srand((unsigned int)time(NULL));
	for (int i = 0; i<ENEMY_NUM; i++)
	{
		enemy[i].init((float)(SCREEN_WIDTH + rand() % +100), rand() % -SCREEN_HEIGHT);
	}
	for (int i = 0; i < 5; i++)
	{
		enemy2[i].init((float)(SCREEN_WIDTH + rand() % +100), rand() % -SCREEN_HEIGHT);
	}
	//총알 초기화
	H_N_bullet[0].init(hero.x_pos, hero.y_pos);
	hitbox.init(hero.x_pos, hero.y_pos);


}

// 메인 게임 로직


void do_game_logic(void)
{
	//주인공 처리 
	if (KEY_DOWN(VK_UP))
		hero.move(MOVE_UP);

	if (KEY_DOWN(VK_DOWN))
		hero.move(MOVE_DOWN);

	if (KEY_DOWN(VK_LEFT))
		hero.move(MOVE_LEFT);

	if (KEY_DOWN(VK_RIGHT))
		hero.move(MOVE_RIGHT);

	nomovingwindow(); //창안에서만 이동
	hitbox.init(hero.x_pos, hero.y_pos); // 피격범위 동기화


	srand((unsigned int)time(NULL) * 100);


	enemy[10].init(1050, 200);
	enemy[11].init(1000, 250);
	enemy[12].init(1100, 300);
	enemy[13].init(1000, 350);
	enemy[14].init(1050, 400);
	for (int i = 0; i < ENEMY_NUM; i++) //에너미 1
	{
		srand((unsigned int)time(NULL) * 100);
		if (enemy[i].x_pos < -SCREEN_HEIGHT)
			enemy[i].init((float)(SCREEN_WIDTH + rand() % +100), rand() % -SCREEN_HEIGHT);
		else
			enemy[i].move(6);
			enemy[i].move2();
	}

	for (int i = 0; i < 5; i++)
	{
		srand((unsigned int)time(NULL) * 100);
		if (enemy2[i].x_pos < -SCREEN_HEIGHT)
			enemy2[i].init((float)(SCREEN_WIDTH + rand() % +100), rand() % -SCREEN_HEIGHT);
		else
		{
			enemy2[i].move(3);
		}
	}
	
		enemy[16].init(0, 10 - 500);
		enemy[17].init(50, 10 - 550);
		enemy[18].init(100, 10 - 600);
		enemy[19].init(150, 10 - 550);
		enemy[20].init(200, 10 - 500);

	// 보스

		for (int k = 0; k < 5; k++)
		{
			misslemove(e_bullet_missle[k].x_pos, e_bullet_missle[k].y_pos, hero.x_pos, hero.y_pos, E_BULLET_SPEED);
		}
	
	if (playerkillcount >50)
	{
		n_boss.init(600, 300);
		n_boss.active();
		Boss_Check = true;

		for (int i = 0; i < ENEMY_BULLET_NUM; i++)
		{
			e_bullet2[i].init(n_boss.x_pos + 32, n_boss.y_pos - 100);
			e_bullet2[i].move(8);
		}
			for (int k = 0; k < 5; k++)
			{
			e_bullet_missle[k].init(n_boss.x_pos + 32, n_boss.y_pos - 100);
			e_bullet_missle[k].move(8);
		}
		
	}
	// 아이템 처리
	srand((unsigned int)time(NULL));
	if (item.x_pos > -SCREEN_WIDTH)
		item.init((float)(rand() % 200 - 300), (float)(rand() % 300));
	else
	{
		item.move(4);
	}

	//총알 처리 

	for (int i = 0; i < HERO_BULLET_NUM; ++i)
	{
		H_N_bullet[i].g_pbullet = H_N_bullet[0].g_pbullet;
		H_N_bullet[i].g_pbullet2 = H_N_bullet[0].g_pbullet2;
		H_N_bullet[i].g_pbullet3 = H_N_bullet[0].g_pbullet3;
		H_N_bullet[i].g_pbarrior = H_N_bullet[0].g_pbarrior;
	}
	/*
	if (KEY_DOWN(VK_SPACE))
	{
		hero.power++;
		if (hero.power < 3)
		{
			hero.power = 2;
		}
	}*/
	if (KEY_DOWN(0x5a))

	{
		for (int i = 0; i < HERO_BULLET_NUM; ++i)
		{
			mciSendCommand(i, MCI_PLAY, MCI_SEEK_TO_START, (DWORD)(LPVOID)&efplay);
			if (H_N_bullet[i].show() == false)
			{
				H_N_bullet[i].active();
				H_N_bullet[i].init(hero.x_pos + 30, hero.y_pos + 10);
				break;
				MessageBeep(1);
			}
		}
	}

	for (int i = 0; i < HERO_BULLET_NUM; ++i)
	{
		if (H_N_bullet[i].show() == true)
		{
			if (H_N_bullet[i].y_pos < -70 || H_N_bullet[i].x_pos >SCREEN_WIDTH + 100)
				H_N_bullet[i].hide();
			else
				H_N_bullet[i].move();
		}
	}

	for (int i = 0; i < ENEMY_BULLET_NUM; i++)
	{

			srand((unsigned int)time(NULL) * 100);
			if (e_bullet[i].x_pos < -SCREEN_HEIGHT)
				e_bullet[i].init((float)(SCREEN_WIDTH + rand() % +100), rand() % -SCREEN_HEIGHT);
			else
				e_bullet[i].move(6);
			

	}


	// 탄환 충돌 처리

	for (int i = 0; i < ENEMY_NUM; i++) // 에너미1
	{
		for (int j = 0; j < HERO_BULLET_NUM; j++)
		{
			if (H_N_bullet[j].check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
			{
				enemy[i].init((float)(SCREEN_WIDTH + rand() % +100), rand() % -SCREEN_HEIGHT);

				playerkillcount++;
			}
		}
	}

	for (int i = 0; i < 5; i++) // 에너미2
	{
		for (int j = 0; j < HERO_BULLET_NUM; j++)
		{
			if (H_N_bullet[j].check_collision(enemy2[i].x_pos, enemy2[i].y_pos) == true) // 에너미2
			{
				enemy2[i].HP -= hero.power;
				if (enemy2[i].HP <= 0)
				{
					enemy2[i].init((float)(SCREEN_WIDTH + rand() % +100), rand() % -SCREEN_HEIGHT);
					enemy2[i].HP = 50;
					playerkillcount+=2;
				}
			}
		}
	}
	
	for (int j = 0; j < HERO_BULLET_NUM; j++)//미사일격추
	{
		for (int i = 0; i < 5; i++) {
			if (Boss_Check == true) {
				if (H_N_bullet[j].check_collision(e_bullet_missle[i].x_pos, e_bullet_missle[i].y_pos) == true) // 보스
				{

					e_bullet_missle[1].init(n_boss.x_pos, n_boss.y_pos);

					if (e_bullet_missle[i].HP <= 0)
					{

						for (int k = 0; k < ENEMY_BULLET_NUM; k++) {
							e_bullet2[k].init((float)(n_boss.x_pos), (float)(n_boss.y_pos));
						}
					}
				}
			}
		}
	}
	for (int j = 0; j < HERO_BULLET_NUM; j++)
	{
		if (H_N_bullet[j].check_collision(n_boss.x_pos, n_boss.y_pos) == true) // 보스
		{
			bosshp -= hero.power;

			if ( bosshp  <= 0)
			{
				hero.KillCount += 100;

				Boss_Check = false;

				n_boss.hide();

				n_boss.init(3000, 3000);
				for (int k = 0; k < ENEMY_BULLET_NUM; k++)
				{
					e_bullet2[k].init((float)(n_boss.x_pos), (float)(n_boss.y_pos));

				}
			}
		}
		
	}
	for (int j = 0; j < HERO_BULLET_NUM; j++)
	{
		if (H_N_bullet[j].check_collision(f_boss.x_pos, f_boss.y_pos) == true) // 최종보스
		{
			f_boss.HP -=hero.power;

			if (f_boss.HP <= 0)
			{
				playerkillcount+=200;

				f_boss.HP = BOSS_HP * 2;
				f_boss.init((float)(POSITION_START), (float)(POSITION_START));
			}
		}

	}
	n_boss.move(BMove);
	if (BMove >= 400)
		BossMoveCheck = true;
	else if (BMove <= 0)
		BossMoveCheck = false;

	if (BossMoveCheck == true) {
		n_boss.y_pos -= 5;
		BMove -= 5;
	}
	else if (BossMoveCheck == false)
	{
		BMove += 5;
		n_boss.y_pos += 5;
	}

	
	 if (hero.check_collision(item.x_pos, item.y_pos) == true) // 아이템
		 {
			 item.init((float)(SCREEN_WIDTH + rand() % +100), rand() % -SCREEN_HEIGHT);

			 hero.KillCount += 5;
			 if (playerHP < 5)
			 {
				 playerHP++;
				 MessageBeep(MB_ICONHAND);
			 }
		 }
	}


// 렌더링
void render_frame(HWND hWnd)
{
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);    // // begin sprite drawing with transparency

											 //  배경
	RECT bgrect1;
	SetRect(&bgrect1, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	D3DXVECTOR3 bgcenter1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 bgposition1(bgpos, 0, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(bg_1, &bgrect1, &bgcenter1, &bgposition1, D3DCOLOR_ARGB(255, 255, 255, 255));

	RECT bgrect2;
	SetRect(&bgrect2, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	D3DXVECTOR3 bgposition2(bgpos+800, 0, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(bg_2, &bgrect1, &bgcenter1, &bgposition2, D3DCOLOR_ARGB(255, 255, 255, 255));


	
	//주인공 
	RECT part;
	SetRect(&part, 0, 0, 60, 40);
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position(hero.x_pos, hero.y_pos, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(g_hero, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	/// 하트
		
	if (playerHP >= 3)
	{
		RECT part3;
		SetRect(&part3, 0, 0, 16, 16);
		D3DXVECTOR3 center3(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position3(20, SCREEN_HEIGHT - 50, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(hp1, &part3, &center3, &position3, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		RECT part3;
		SetRect(&part3, 0, 0, 16, 16);
		D3DXVECTOR3 center3(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position3(-20, SCREEN_HEIGHT - 50, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(hp2, &part3, &center3, &position3, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (playerHP >= 2)
	{
		RECT part4;
		SetRect(&part4, 0, 0, 16, 16);
		D3DXVECTOR3 center4(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position4(  20 * 2, SCREEN_HEIGHT - 50, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(hp1, &part4, &center4, &position4, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		RECT part4;
		SetRect(&part4, 0, 0, 16, 16);
		D3DXVECTOR3 center4(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position4( 20 * 2, SCREEN_HEIGHT - 50, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(hp2, &part4, &center4, &position4, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (playerHP >= 1)
	{
		RECT part5;
		SetRect(&part5, 0, 0, 16, 16);
		D3DXVECTOR3 center5(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position5( 20 * 3, SCREEN_HEIGHT - 50, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(hp1, &part5, &center5, &position5, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		RECT part5;
		SetRect(&part5, 0, 0, 16, 16);
		D3DXVECTOR3 center5(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position5( 20 * 3, SCREEN_HEIGHT-50, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(hp2, &part5, &center5, &position5, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	
	////총알 
		for (int i = 0; i < HERO_BULLET_NUM; ++i)
		{
			if (H_N_bullet[i].bShow == true)
			{
				if (hero.power == 1)
				{
					RECT pbulletrect1;
					SetRect(&pbulletrect1, 0, 0, 30, 20);
					D3DXVECTOR3 pbulletcenter1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
					D3DXVECTOR3 pbulletposition1(H_N_bullet[i].x_pos, H_N_bullet[i].y_pos, 0.0f);    // position at 50, 50 with no depth
					d3dspt->Draw(H_N_bullet[i].g_pbullet, &pbulletrect1, &pbulletcenter1, &pbulletposition1, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				else if (hero.power == 2)
				{
					RECT pbulletrect2;
					SetRect(&pbulletrect2, 0, 0, 80, 30);
					D3DXVECTOR3 pbulletcenter2(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
					D3DXVECTOR3 pbulletposition2(H_N_bullet[i].x_pos, H_N_bullet[i].y_pos, 0.0f);    // position at 50, 50 with no depth
					d3dspt->Draw(H_N_bullet[i].g_pbullet2, &pbulletrect2, &pbulletcenter2, &pbulletposition2, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				else if (hero.power <= 3)
				{
					RECT pbulletrect3;
					SetRect(&pbulletrect3, 0, 0, 80, 40);
					D3DXVECTOR3 pbulletcenter3(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
					D3DXVECTOR3 pbulletposition3(H_N_bullet[i].x_pos, H_N_bullet[i].y_pos, 0.0f);    // position at 50, 50 with no depth
					d3dspt->Draw(H_N_bullet[i].g_pbullet3, &pbulletrect3, &pbulletcenter3, &pbulletposition3, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
			}
		}
	////에네미 
	RECT part2;
	SetRect(&part2, 0, 0, 26, 33);
	D3DXVECTOR3 center2(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

	for (int i = 0; i<ENEMY_NUM; i++)
	{
		D3DXVECTOR3 position2(enemy[i].x_pos, enemy[i].y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(g_enemy, &part2, &center2, &position2, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	// 강한 에네미
	RECT part9;
	SetRect(&part9, 0, 0, 160, 100);
	D3DXVECTOR3 center9(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

	for (int i = 0; i < 5; i++)
	{
		D3DXVECTOR3 position9(enemy2[i].x_pos, enemy2[i].y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(g_enemy2, &part9, &center9, &position9, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	// 보스
	if (Boss_Check == true) {
		RECT bossrect;
		SetRect(&bossrect, 0, 0, 400, 280);
		D3DXVECTOR3 bosscenter(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 bossposition(n_boss.x_pos, n_boss.y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(g_boss, &bossrect, &bosscenter, &bossposition, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	// 최종보스
	RECT fbossrect;
	SetRect(&fbossrect, 0, 0, 600, 400);
	D3DXVECTOR3 fbosscenter(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 fbossposition(f_boss.x_pos, f_boss.y_pos, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(g_fboss, &fbossrect, &fbosscenter, &fbossposition, D3DCOLOR_ARGB(255, 255, 255, 255));
	

	//적 총탄총알 
	for (int i = 0; i < HERO_BULLET_NUM; ++i)
	{
		RECT e_bullet1rect;
		SetRect(&e_bullet1rect, 0, 0, 18, 6);
		D3DXVECTOR3 center15(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position15(e_bullet[i].x_pos, e_bullet[i].y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(g_enemy_bullet, &e_bullet1rect, &center15, &position15, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (Boss_Check == true) {
		RECT e_bullet2rect;
		SetRect(&e_bullet2rect, 0, 0, 16, 16);
		D3DXVECTOR3 center16(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

		for (int i = 0; i < ENEMY_BULLET_NUM; ++i)
		{
			D3DXVECTOR3 position16(e_bullet2[i].x_pos, e_bullet2[i].y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(g_enemy_bullet2, &e_bullet2rect, &center16, &position16, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		RECT e_bullet3rect;
		SetRect(&e_bullet3rect, 0, 0, 16, 16);
		D3DXVECTOR3 center17(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

		for (int i = 0; i < 5; ++i)
		{
			D3DXVECTOR3 position17(e_bullet_missle[i].x_pos, e_bullet_missle[i].y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(g_enemy_bullet3_missle, &e_bullet3rect, &center17, &position17, D3DCOLOR_ARGB(255, 255, 255, 255));

		}
	}
	RECT e_bullet4rect;
	SetRect(&e_bullet4rect, 0, 0, 26, 26);
	D3DXVECTOR3 center18(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position18(e_bullet_missle2.x_pos, e_bullet_missle2.y_pos, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(g_enemy_bullet4_missle2, &e_bullet4rect, &center18, &position18, D3DCOLOR_ARGB(255, 255, 255, 255));

	RECT e_bullet5rect;
	SetRect(&e_bullet5rect, 0, 0, 48, 38);
	D3DXVECTOR3 center11(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position11(e_bullet_laser.x_pos, e_bullet_laser.y_pos, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(g_enemy_bullet5_laser, &e_bullet5rect, &center11, &position11, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 아이템
	RECT itemrect1;
	SetRect(&itemrect1, 0, 0, 20, 20);
	D3DXVECTOR3 itemrectcenter1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 itemrectpos1(item.x_pos, item.y_pos, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(item_hp, &itemrect1, &itemrectcenter1, &itemrectpos1, D3DCOLOR_ARGB(255, 255, 255, 255));


	RECT itemrect2;
	SetRect(&itemrect2, 0, 0, 20, 20);
	D3DXVECTOR3 itemrectcenter2(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 itemrectpos2(item.x_pos, item.y_pos, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(item_pow, &itemrect2, &itemrectcenter2, &itemrectpos2, D3DCOLOR_ARGB(255, 255, 255, 255));

	RECT itemrect3;
	SetRect(&itemrect3, 0, 0, 20, 20);
	D3DXVECTOR3 itemrectcenter3(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 itemrectpos3(item.x_pos, item.y_pos, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(item_sp, &itemrect3, &itemrectcenter3, &itemrectpos3, D3DCOLOR_ARGB(255, 255, 255, 255));

	

	d3dspt->End();    // end sprite drawing

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}

// 반환
void cleanD3D(void)
{
	d3ddev->Release();
	d3d->Release();

	//객체 해제
	g_hero->Release();
	g_hero2->Release();
	g_enemy->Release();
	g_enemy2->Release();
	H_N_bullet[0].g_pbullet->Release();
	H_N_bullet[0].g_pbullet2->Release();
	H_N_bullet[0].g_pbullet3->Release();
	H_N_bullet[0].g_pbarrior->Release();
	g_boss->Release();
	hp1->Release();
	hp2->Release();
	
	bg_1->Release();
		bg_2->Release();;
	g_enemy_bullet->Release();
	g_enemy_bullet2->Release();
	g_enemy_bullet3_missle->Release();
	g_enemy_bullet4_missle2->Release();
	g_enemy_bullet5_laser->Release();
	//clear->Release();

	return;
}

// 주인공 피격
void playerHit()
{
	if (hitbox.check_collision(item.x_pos, item.y_pos) == true) //회복 아이템
	{
		if (playerHP < 3)
		{
			playerHP++;
		}
		item.init((float)(rand() % 200 - 300), (float)(rand() % 300));
	}

	//적에게 직접 충돌

	for (int i = 0; i < 20; i++) // 일반적 충돌
	{
		if (hero.check_collision(enemy2[i].x_pos, enemy2[i].y_pos) == true)
		{
			if (playerHP > 0)
			{
				playerHP--;

			}
		}
	}
	if (hero.check_collision(n_boss.x_pos, n_boss.y_pos) == true)
	{
		if (playerHP > 0)
		{
			playerHP--;
		}
	}
	if (hero.check_collision(f_boss.x_pos, f_boss.y_pos) == true)
	{
		if (playerHP> 0)
		{
			playerHP--;
		}
	}

	//적 총알 피격
	for (int i = 0; i < 5; ++i)
	{
		if (hero.check_collision(e_bullet_missle[i].x_pos, e_bullet_missle[i].y_pos) == true)
		{
			if (playerHP > 0)
			{
				playerHP--;
			}
		}
	}
	for (int i = 0; i < ENEMY_BULLET_NUM; ++i)
	{
		if (hero.check_collision(e_bullet[i].x_pos, e_bullet[i].y_pos) == true) 
		{
			if (playerHP > 0)
			{
				playerHP--;
			}
		}

		if (hero.check_collision(e_bullet2[i].x_pos, e_bullet2[i].y_pos) == true)
		{
			if (hero.HP > 0)
			{
				hero.HP--;
			}
		}
	}
	if (hero.check_collision(e_bullet_laser.x_pos, e_bullet_laser.y_pos) == true) //레이저
	{
		if (playerHP > 0)
		{
			playerHP--;
		}
	}
	if (hero.check_collision(e_bullet_missle2.x_pos, e_bullet_missle2.y_pos) == true)
	{
		if (playerHP > 0)
		{
			playerHP--;
		}
	}
	for (int i = 0; i < ENEMY_NUM; i++) // 일반적 충돌
	{
		if (hero.check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
		{
			if (playerHP > 0)
			{
				playerHP--;
			}
		}
	}
}


// 이동 거리 제한
void nomovingwindow(void)
{
	if (hero.x_pos > (SCREEN_WIDTH - PLAYER_SIZE))
		hero.move(MOVE_LEFT);
	if (hero.x_pos < 0)
		hero.move(MOVE_RIGHT);
	if (hero.y_pos < 0)
		hero.move(MOVE_DOWN);
	if (hero.y_pos > SCREEN_HEIGHT - PLAYER_SIZE)
		hero.move(MOVE_UP);
}
void gameover() {

	if (playerkillcount > 300) {


 }
}

