#include <cmath>

void misslemove(float& x, float& y, float px, float py, float speed) // źȯ�� ��ǥ, ���� ĳ������ ��ǥ, źȯ�� �ӵ�
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

void E_Bullet2_move(float& x, float& y, float moving, float& vx, float& vy) // źȯ�� ��ǥ, ���ӵ�, �ӵ� ������ ��ǥ
{
	vy += 0.12;
	vx += moving;
	x += vx;
	y += vy;
}
