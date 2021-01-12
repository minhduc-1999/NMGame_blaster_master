#pragma once
#include "Panel.h"
#include "Sprites.h"
class SlidePanel :public Panel
{
	CAnimation* anim;
public:
	SlidePanel(float x, float y, float vx, float vy, int w, int h, CAnimation* anim);
	void Update(unsigned long dt);
	void Render();
};