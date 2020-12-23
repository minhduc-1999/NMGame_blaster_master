#include "HPBar.h"
#include <cmath>

HPBar::HPBar() :CDynamicGameObject(x, y)
{
	shp = SOPHIA_MAX_HP;
	jhp = JASON_MAX_HP;
	hp = 8;
	SetSize(45, 233);

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(50);
	this->SetAnimationSet(ani_set);
}

void HPBar::Update(DWORD dt, float x, float y)
{
	this->SetPosition(x, y);
	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();

	//CalcPotentialCollisions(coObjects, coEvents);
	CDynamicGameObject::Update(dt);
}

void HPBar::Render()
{
	int ani = HP_BAR_FULL;
	if (hp > 8)
		hp = 8;
	switch (hp)
	{
	case 8:
		ani = HP_BAR_FULL;
		break;
	case 7:
		ani = HP_BAR_7;
		break;
	case 6:
		ani = HP_BAR_6;
		break;
	case 5:
		ani = HP_BAR_5;
		break;
	case 4:
		ani = HP_BAR_4;
		break;
	case 3:
		ani = HP_BAR_3;
		break;
	case 2:
		ani = HP_BAR_2;
		break;
	case 1:
		ani = HP_BAR_1;
		break;
	case 0:
		ani = HP_BAR_0;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(x, y, -1);
}
void HPBar::SetState(int bar)
{
	CDynamicGameObject::SetState(bar);
	switch (bar)
	{
	case SOPHIA_HPBAR:
		hp = (shp+1)/2;
		curBar = 0;
		break;
	case JASON_HPBAR:
		hp = jhp;
		curBar = 1;
		break;
	case HP_UP:
		if (curBar == 0)
		{
			shp += 2;
			hp = shp;
			this->SetState(SOPHIA_HPBAR);
		}
		else
		{
			jhp++;
			hp = jhp;
			this->SetState(JASON_HPBAR);
		}
		break;
	case HP_DOWN:
		if (curBar == 0)
		{
			shp--;
			hp = (shp+1)/2;
			this->SetState(SOPHIA_HPBAR);
		}
		else
		{
			jhp--;
			hp=jhp;
			this->SetState(JASON_HPBAR);
		}
		break;
	default:
		break;
	}
}
