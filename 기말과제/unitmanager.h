#include "gamemanager.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define ENEMY_NUM 20
#define ENEMY_BULLET_NUM 300
#define HERO_BULLET_NUM 200

#define E_MOVE 30

#define MOVE_SPEED 8
#define BULLET_SPEED 30
#define E_BULLET_SPEED 5
#define PLAYER_SIZE 30

#define ENEMY1_HP 2

#define ENEMY2_HP 10
#define BOSS_HP 600

#define FBOSS_HP 2000
#define POSITION_START 1500
//브금

MCI_OPEN_PARMS bgmfileopen;
MCI_PLAY_PARMS bgmPlay;


//---------------------------스프라이트 지정
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPD3DXSPRITE d3dspt;    // the pointer to our Direct3D Sprite interface
HINSTANCE g_hInst;
// sprite declarations
LPDIRECT3DTEXTURE9 g_hero;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 g_hero2;    // the pointer to the sprite


LPDIRECT3DTEXTURE9 g_enemy;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 g_enemy2;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 g_enemy3;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 g_enemy4;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 g_boss;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 g_fboss;    // the pointer to the sprite



LPDIRECT3DTEXTURE9 g_enemy_bullet;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 g_enemy_bullet2;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 g_enemy_bullet3_missle;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 g_enemy_bullet4_missle2;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 g_enemy_bullet5_laser;    // the pointer to the sprite


LPDIRECT3DTEXTURE9 fire;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 clear;    // the pointer to the sprite

LPDIRECT3DTEXTURE9 hp1;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 hp2;    // the pointer to the sprite

LPDIRECT3DTEXTURE9 item_pow;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 item_hp;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 item_sp;

LPDIRECT3DTEXTURE9 score;    // the pointer to the sprite

LPDIRECT3DTEXTURE9 bg_1;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 bg_2;    // the pointer to the sprite

LPDIRECT3DTEXTURE9 pressbutton;    // the pointer to the sprite


bool  BossCheck1 = true;
bool  BossCheck2 = true;
bool  BossCheck3 = true;

float  BossMove = 0.0f;
bool BossMoveCheck = true;

//-------------------함수 정의-----------------------------------




enum { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
//충돌 체크
bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1)
{

	if ((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) < (size0 + size1) * (size0 + size1))
		return true;
	else
		return false;
}

//기본
class entity {

public:
	float x_pos;
	float y_pos;
	int status;
	int HP;
	int KillCount; // 점수
	int BossEvent; // 보스 등장 조건 점수
};


class Image
{
	int frame_width;

	int frame_height;

	bool m_animate;

	void Doani(float dt);

	//	BOOL				Visible;	// 보이기 여부
	LPDIRECT3DTEXTURE9	Texture;	// 텍스쳐
	RECT				Source;		// 텍스쳐내 이미지의 좌표
	D3DXVECTOR3			Position;	// 이미지가 화면에 그려질 좌표
	D3DXVECTOR3			Center;		// 이미지의 센터
	float				Sec;		// 재생시간

};
RECT playeranirect[] = { { 0,0,60,40 } ,{ 61,0,122,40 } ,{ 123,0,184,40 } ,{ 185,0,240,40 } };


struct Property
{
	float Speed;			// 이동속도
	float AttackDamage;		// 공격력

	bool ALive;				// 살아있는 상태
};



class Player :public entity {
public:
	void updateKey(void);
	void init(float x, float y);
	void move(int i);
	float power;
	void fire();
	void subfire();
	void wpchange();
	void barrior();


	bool check_collision(float x, float y);
};

void Player::init(float x, float y)
{
	power = 1;
	x_pos = x;
	y_pos = y;
}




void Player::move(int i)
{
	switch (i)
	{
	case MOVE_UP:
		y_pos -= MOVE_SPEED;
		break;
	case MOVE_DOWN:
		y_pos += MOVE_SPEED;
		break;
	case MOVE_LEFT:
		x_pos -= MOVE_SPEED;
		break;
	case MOVE_RIGHT:
		x_pos += MOVE_SPEED;
		break;
	}
}

bool Player::check_collision(float x, float y)
{
	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32, x, y, 32) == true)
	{
		HP--;
		return true;
	}
	else
	{
		return false;
	}
}
//아이템 클래스
class Item :public entity {
public:
	void init(float x, float y);
	void move(int i);
	bool check_collision(float x, float y);

};


bool Item::check_collision(float x, float y)
{
	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32, x, y, 32) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
// 적 클래스
class Enemy :public entity {

public:
	void init(float x, float y);
	void move(int E_speed);
	void move2();
};

void Enemy::init(float x, float y)
{
	x_pos = x;
	y_pos = y;
}

void Enemy::move(int E_speed)
{
	x_pos -= E_speed;

	
}

void Enemy::move2()
{

		y_pos -= rand() %3;
		y_pos += rand() % 3;
}

// 보스 클래스
class Boss :public entity {

public:
	bool bossspown;
	bool active();
	bool hide();
	void init(float x, float y);
	void move(float x);
	
};

bool Boss::active()
{
	Boss_Check = true;
	return bossspown;
}

bool Boss::hide()
{
	Boss_Check = false;
	return bossspown;
}
void Boss::init(float x, float y)
{
	x_pos = x;
	y_pos = y;
}

void Boss::move(float x){
	y_pos = BMove;
}


class Enemy_Bullet :public entity {

public:
	float vx, vy;

	void init(float x, float y);
	void move(int EBspeed);
};

void Enemy_Bullet::init(float x, float y)
{
	x_pos = x;
	y_pos = y;
}

void Enemy_Bullet::move(int EBspeed)
{
	x_pos -= EBspeed;
}

// 총알 클래스 
class Bullet :public entity {

public:
	bool bShow;
	LPDIRECT3DTEXTURE9 g_pbullet;    // the pointer to the sprite
	LPDIRECT3DTEXTURE9 g_pbullet2;    // the pointer to the sprite
	LPDIRECT3DTEXTURE9 g_pbullet3;    // the pointer to the sprite
	LPDIRECT3DTEXTURE9 g_pbarrior;    // the pointer to the sprite
	void init(float x, float y);
	void move();
	bool show();
	void hide();
	void active();
	bool check_collision(float x, float y);

};

bool Bullet::check_collision(float x, float y)
{
	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32, x, y, 32) == true)
	{
		bShow = false;
		return true;
	}
	else
	{
		return false;
	}
}


void Bullet::init(float x, float y)
{
	x_pos = x;
	y_pos = y;

}

bool Bullet::show()
{
	return bShow;
}

void Bullet::active()
{
	bShow = true;
}

void Bullet::move()
{
	x_pos += BULLET_SPEED;
}

void Bullet::hide()
{
	bShow = false;
}

