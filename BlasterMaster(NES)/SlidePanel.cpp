#include "SlidePanel.h"

SlidePanel::SlidePanel(float x, float y,float vx, float vy, int w, int h, CAnimation* anim)
	:Panel(x, y, vx, vy)
{
	this->width = w;
	this->height = h;
	this->anim = anim;
	this->anim->ResetAnim();
}
void SlidePanel::Update(unsigned long dt)
{
	
}
void SlidePanel::Render()
{
	anim->Render(x, y, -1);
	if (anim->IsCompleted())
		Finish();
}