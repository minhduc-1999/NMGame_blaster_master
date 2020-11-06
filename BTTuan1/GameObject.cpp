#include "GameObject.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

CGameObject::CGameObject(float x, float y) :CGameObject()
{
	SetPosition(x, y);
}


void CGameObject::AddAnimation(int aniId)
{
	LPANIMATION ani = CAnimationManager::GetInstance()->Get(aniId);
	animations.push_back(ani);
}

