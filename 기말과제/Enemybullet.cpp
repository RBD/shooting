#include <cmath>

void misslemove(float& x, float& y, float px, float py, float speed) // 탄환의 좌표, 메인 캐릭터의 좌표, 탄환의 속도
{
	float d = sqrt((px - x)*(px - x) + (py - y)*(py - y)); 

	float vx, vy;

	if (d)
	{
		vx = (px - x) / d*speed;
		vy = (py - y) / d*speed;
	}
	else
	{
		vx = 0;
		vy = speed;
	}
	x += vx;
	y += vy;
}

void E_Bullet2_move(float& x, float& y, float moving, float& vx, float& vy) // 탄환의 좌표, 가속도, 속도 벡터의 좌표
{
	vy += 0.12;
	vx += moving;
	x += vx;
	y += vy;
}
