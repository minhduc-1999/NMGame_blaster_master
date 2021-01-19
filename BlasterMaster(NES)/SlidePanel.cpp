#include "SlidePanel.h"

SlidePanel::SlidePanel(float x, float y, float vx, float vy, int w, int h, CAnimation* anim)
	:Panel(x, y, vx, vy)
{
	this->width = w;
	this->height = h;
	this->anim = anim;
	this->anim->ResetAnim();
	this->period = 0;
	mounx = 132.0f;
	mouny = 124.0f;
	quakeTime = 0;
	flag = true;
}
void SlidePanel::Update(unsigned long dt)
{
	
	if (period == 0)
	{
		if (GetTickCount64() - quakeTime >= 30) {
			flag = !flag;
			quakeTime = GetTickCount64();
		}
			
		mouny += 0.02f * dt;
		if (mouny >= 150)
		{
			period = 1;
		}
	}
}
void SlidePanel::Render()
{
	if (period == 0)
	{
		CSpriteManager::GetInstance()->Get(6000)->Draw(mounx, mouny, -1, 255);
		if (this->flag == true)
		{
			//DebugOut("[rung 1]\n");
			CSpriteManager::GetInstance()->Get(1005)->Draw(x, y - 1, -1, 255);
		}
		else
		{
			//DebugOut("[rung 2]\n");
			CSpriteManager::GetInstance()->Get(1005)->Draw(x, y + 1, -1, 255);
		}
	}
	else if (period == 1)
	{
		anim->Render(x, y, -1);
		if (anim->IsCompleted())
			Finish();
	}
}
