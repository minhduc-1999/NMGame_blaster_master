#include "MoviePanel.h"

MoviePanel::MoviePanel(float x, float y, float vx, float vy, int w, int h, CSprite* sp)
	:Panel(x, y, vx, vy)
{
	this->width = w;
	this->height = h;
	this->sprite = sp;
}
void MoviePanel::Update(unsigned long dt)
{
	if (!done)
	{
		x += vx * dt;
		y += vy * dt;
		if (x < 0)
		{
			Finish();
			x = 0;
		}
	}
}
void MoviePanel::Render()
{
	sprite->Draw(x, y, -1, 255);
	CAnimationManager::GetInstance()->Get(1)->Render(x + 160, 95, -1);
	CSpriteManager::GetInstance()->Get(3000)->Draw(x + 174, 115, -1, 255);
}
bool MoviePanel::IsDone()
{
	if (done && GetTickCount64() - timeDone > 4000)
		return true;
	return false;
}