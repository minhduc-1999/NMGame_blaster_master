#include "Item.h"

Item::Item(float x, float y) : CDynamicGameObject(x, y)
{
	SetSize(16, 16);
	SetAnimationSet(ITEM_HP_ANISET);
	vx = vy = 0;
	SetType(26);
	SetPosition(x, y);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ITEM_HP_ANISET);
	this->SetAnimationSet(ani_set);
	isUpdated = false;
	isRendered = false;
}

int Item::Update(DWORD dt, vector<LPGAMEOBJECT> coObjects)
{
	if (isUpdated)
		return -1;

}

void Item::Render()
{
	animation_set->at(0)->Render(x, y, -1);
}