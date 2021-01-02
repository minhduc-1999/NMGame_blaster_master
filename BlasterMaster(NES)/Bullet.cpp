#include "Bullet.h"

Bullet::Bullet(float x, float y, int type, int n) : CDynamicGameObject(x, y)
{
	startFiringTime = GetTickCount();
	isDestroyed = false;

	if (type == SOPHIA_BULLET_HORIZONTAL)
	{
		SetSize(6, 6);

		nx = n;
		vx = -(float)n * 0.2f;
		vy = 0;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_SOPHIA_HOR);
		this->SetAnimationSet(ani_set);
	}
	else if (type == SOPHIA_BULLET_VERTICAL)
	{
		SetSize(8, 26);

		nx = n;
		vx = 0;
		vy = -0.2f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_SOPHIA_VER);
		this->SetAnimationSet(ani_set);
	}
	else if (type == CANNON_BULLET_HORIZONTAL)
	{
		SetSize(8, 8);

		nx = 1;
		vx = n * 0.12f;
		vy = 0;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_RED);
		this->SetAnimationSet(ani_set);
	}
	else if (type == CANNON_BULLET_VERTICAL)
	{
		SetSize(8, 8);

		nx = 1;
		vx = 0;
		vy = n * 0.12f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_RED);
		this->SetAnimationSet(ani_set);
	}
	else if (type == FLOATER_BULLET)
	{
		SetSize(6, 6);

		nx = 1;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_FLOATER);
		this->SetAnimationSet(ani_set);
	}
	else if (type == EYEBALL_BULLET)
	{
		SetSize(8, 8);

		nx = 1;
		vx = 0.08f;
		vy = 0.08f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_RED);
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

void Bullet::SetSpeed(float spX, float spY)
{
	vx = spX;
	vy = spY;
}