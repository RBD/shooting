
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <strsafe.h>
#include <MMSystem.h>
#include <Digitalv.h>

#include"resource.h"
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(HWND hWnd);    // renders a single frame
void cleanD3D(void);		// closes Direct3D and releases memory
void init_game(void);
void do_game_logic(void);
bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1);

bool Boss_Check = false;
void playerHit();//Ã³¸Â
void gameover();

void nomovingwindow(void);//


float BMove = 0.0f;





void misslemove(float& x, float& y, float px, float py, float speed);


MCI_OPEN_PARMS bgmopen;
MCI_PLAY_PARMS bgmplay;
MCI_OPEN_PARMS efopen;
MCI_PLAY_PARMS efplay;

int bgmID;
int bgmID2;
int efID;

int bgpos = 0;
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

using namespace std;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		cleanD3D();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


float E_Bulletrand = 0;

#pragma once
