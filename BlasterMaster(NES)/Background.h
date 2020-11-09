#pragma once
#include "Sprites.h"
#include "Utils.h"

class CBackground
{
protected:
	float x, y;
	float width, height;
	LPSPRITE sprite;
public:
	CBackground(float x, float y);
	void SetSize(float width, float height);
	void SetPosition(float x, float y);
	void SetSprite(LPSPRITE sp);
	void Render();
	Rect GetBound();
};
typedef CBackground* LPBACKGROUND;

