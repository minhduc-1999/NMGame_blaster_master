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
