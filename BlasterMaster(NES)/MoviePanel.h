#pragma once
#include "Panel.h"
#include "Sprites.h"
class MoviePanel :public Panel
{
	CSprite* sprite;
public:
	MoviePanel(float x, float y, float vx, float vy, int w, int h, CSprite* sp);
	void Update(unsigned long dt);
	void Render();
	bool IsDone();
};
