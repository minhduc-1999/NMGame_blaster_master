#include "NotifyPanel.h"

NotifyPanel::NotifyPanel(float x, float y, float vx, float vy, int w, int h, CSprite* sp)
	:Panel(x, y, vx, vy)
{
	this->width = w;
	this->height = h;
	this->sprite = sp;
}
void NotifyPanel::Update(unsigned long dt)
{
	if (!done)
	{
		Finish();
	}
}
void NotifyPanel::Render()
{
	sprite->Draw(x, y, -1, 255);
}
bool NotifyPanel::IsDone()
{
	if (done && GetTickCount64() - timeDone > NOTIFY_TIME)
		return true;
	return false;
}