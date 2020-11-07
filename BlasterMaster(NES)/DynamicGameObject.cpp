#include "DynamicGameObject.h"

CDynamicGameObject::CDynamicGameObject(float x, float y) :CGameObject(x, y)
{
	vx = vy = 0;
	nx = 1;
}

void CDynamicGameObject::SetSpeed(float vx, float vy)
{
	this->vx = vx;
	this->vy = vy;
}

void CDynamicGameObject::SetState(int state)
{
	this->state = state;
}

int CDynamicGameObject::GetState()
{
	return this->state;
}

D3DXVECTOR3 CDynamicGameObject::GetSpeed()
{
	return D3DXVECTOR3(vx, vy, 0);
}

void CDynamicGameObject::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CDynamicGameObject::AddAnimation(int aniId)
{
	LPANIMATION ani = CAnimationManager::GetInstance()->Get(aniId);
	animations.push_back(ani);
}