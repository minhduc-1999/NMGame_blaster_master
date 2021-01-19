#pragma once
#include "Panel.h"
#include "Sprites.h"

#define NOTIFY_TIME	2000
class NotifyPanel :public Panel
{
	CSprite* sprite;
public:
	NotifyPanel(float x, float y, float vx, float vy, int w, int h, CSprite* sp);
	void Update(unsigned long dt);
	void Render();
	bool IsDone();
};