#include "HPBar.h"
#include <cmath>

HPBar::HPBar() :CDynamicGameObject(x, y)
{
	hp = 8;
	SetSize(45, 233);

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(50);
	this->SetAnimationSet(ani_set);
}

void HPBar::Update(DWORD dt, float x, float y, int curHP)
{
	this->SetPosition(x, y);
	hp = curHP;
	CDynamicGameObject::Update(dt);
	return 0;
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
}
