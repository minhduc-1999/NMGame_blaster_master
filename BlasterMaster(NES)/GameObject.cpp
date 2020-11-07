#include "GameObject.h"


CGameObject::CGameObject(float x, float y)
{
	SetPosition(x, y);
}

void CGameObject::SetSize(float width, float height)
{
	this->width = width; this->height = height;
}

void CGameObject::SetPosition(float x, float y)
{
	this->x = x; this->y = y;
}
D3DXVECTOR3 CGameObject::GetPosition()
{
	return D3DXVECTOR3(x, y, 0);
}

D3DXVECTOR3 CGameObject::GetSize()
{
	return D3DXVECTOR3(width, height, 0);
}

RECT CGameObject::GetBound()
{
	RECT bound;

	bound.left = x - width / 2;
	bound.right = bound.left + width - 1;
	bound.top = y - height / 2;
	bound.bottom = bound.top + height - 1;

	return bound;
}
