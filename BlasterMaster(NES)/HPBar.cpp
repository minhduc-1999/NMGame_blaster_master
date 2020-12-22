#include "HPBar.h"
#include <cmath>

HPBar::HPBar(float x, float y) :CDynamicGameObject(x, y)
{
	shp = SOPHIA_MAX_HP;
	jhp = JASON_MAX_HP;
	hp = JASON_MAX_HP;
	SetSize(45, 233);
}

void HPBar::Update(DWORD dt)
{
	/*if (isUpdated)
		return;*/
	/*hp--;
	if (hp == 0)
		hp = JASON_MAX_HP;*/
	CDynamicGameObject::Update(dt);
}

void HPBar::Render()
{
	if (isRendered)
		return;
	int ani = HP_BAR_FULL;
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

	animation_set->at(ani)->Render(x, y, 1);
	isRendered = true;
	isUpdated = false;
}
void HPBar::SetState(int bar)
{
	CDynamicGameObject::SetState(bar);
	switch (bar)
	{
	case SOPHIA_HPBAR:
		hp = shp;
		curBar = 0;
		break;
	case JASON_HPBAR:
		hp = jhp;
		curBar = 1;
		break;
	case HP_UP:
		hp++;
		if (curBar == 0)
			shp = hp*2;
		else
			jhp = hp;
		break;
	case HP_DOWN:
		hp--;
		if (curBar == 0)
			shp = hp * 2;
		else
			jhp = hp;
		break;
	default:
		break;
	}
}
