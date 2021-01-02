#include "GameObject.h"
#include "Textures.h"


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

void CGameObject::RenderBoundingBox()
{
	Rect rect;

	LPDIRECT3DTEXTURE9 bbox = CTextureManager::GetInstance()->Get(-100);

	Rect bound = this->GetBound();

	rect.left = 0;
	rect.top = 0;
	rect.right = bound.right - bound.left;
	rect.bottom = bound.bottom - bound.top;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, -1);
}