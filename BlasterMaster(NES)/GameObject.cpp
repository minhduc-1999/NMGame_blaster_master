#include "GameObject.h"


CGameObject::CGameObject(float x, float y)
{
	SetPosition(x, y);
	vx = 0.0f;
	vy = 0.0f;
}

void CGameObject::SetSize(float width, float height)
{
	this->width = width; this->height = height;
}

void CGameObject::SetPosition(float x, float y)
{
	this->x = x; this->y = y;
}
D3DXVECTOR2 CGameObject::GetPosition()
{
	return D3DXVECTOR2(x, y);
}

D3DXVECTOR2 CGameObject::GetSize()
{
	return D3DXVECTOR2(width, height);
}


Rect CGameObject::GetBound()
{
	Rect bound;

	bound.left = x - width / 2;
	bound.right = bound.left + width - 1;
	bound.top = y - height / 2;
	bound.bottom = bound.top + height- 1;

	return bound;
}

void CGameObject::SetSpeed(float vx, float vy)
{
	this->vx = vx;
	this->vy = vy;
}

D3DXVECTOR2 CGameObject::GetSpeed()
{
	return D3DXVECTOR2(vx, vy);
}