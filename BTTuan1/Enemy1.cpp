#include "Enemy1.h"

Enemy1::Enemy1(float x, float y) :CGameObject(x, y)
{
};

void Enemy1::Update(DWORD dt)
{
	CGameObject::Update(dt);
}

void Enemy1::Render()
{
	LPANIMATION ani = animations[0];
	ani->Render(x, y, 1);
}

void Enemy1::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case IDLE:
		vx = 0;
		vy = 0;
		break;
	case GO_LEFT:
		vx = -SPEED;
		vy = 0;
		nx = -1;
		break;
	case GO_RIGHT:
		vx = SPEED;
		vy = 0;
		nx = 1;
		break;
	case GO_TOP:
		vx = 0;
		vy = -SPEED;
		break;
	case GO_BOTTOM:
		vx = 0;
		vy = SPEED;
		break;
	default:
		break;
	}
}

