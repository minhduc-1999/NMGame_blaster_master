#include "Background.h"

CBackground::CBackground(float x, float y)
{
	this->x = x;
	this->y = y;
	width = 16.0f;
	height = 16.0f;
}

void CBackground::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;
}

void CBackground::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CBackground::SetSprite(LPSPRITE sp)
{
	sprite = sp;
}

void CBackground::Render()
{
	sprite->Draw(x, y, -1);
}

Rect CBackground::GetBound()
{
	Rect bound;

	bound.left = x - width / 2;
	bound.right = bound.left + width - 1;
	bound.top = y - height / 2;
	bound.bottom = bound.top + height - 1;

	return bound;
}
