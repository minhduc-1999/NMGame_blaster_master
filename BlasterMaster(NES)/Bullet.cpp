#include "Bullet.h"

Bullet::Bullet(float x, float y, int type, int n) : CDynamicGameObject(x, y)
{
	startFiringTime = GetTickCount();
	isDestroyed = false;

	if (type == BULLET_HORIZONTAL)
	{
		SetSize(26, 8);

		nx = n;
		vx = -(float)n * 0.2f;
		vy = 0;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(20);
		this->SetAnimationSet(ani_set);
	}
	else
	{
		SetSize(8, 26);

		nx = n;
		vx = 0;
		vy = -0.2f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(21);
		this->SetAnimationSet(ani_set);
	}
}

void Bullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		isDestroyed = true;
	}

	if (GetTickCount() - startFiringTime >= 1000)
		isDestroyed = true;

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Bullet::Render()
{
	int ani = BULLET_FIRING;

	animation_set->at(ani)->Render(x, y, nx);
}

void Bullet::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	nx = -1;
}