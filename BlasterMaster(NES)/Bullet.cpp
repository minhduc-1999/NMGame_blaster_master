#include "Bullet.h"

Bullet::Bullet(float x, float y, int t, int n) : CDynamicGameObject(x, y)
{
	startFiringTime = GetTickCount();
	isDestroyed = false;
	bulletType = t;
	SetType(20);

	if (t == SOPHIA_BULLET_HORIZONTAL)
	{
		SetSize(6, 6);

		nx = n;
		vx = -(float)n * 0.2f;
		vy = 0;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_SOPHIA_HOR);
		this->SetAnimationSet(ani_set);
	}
	else if (t == SOPHIA_BULLET_VERTICAL)
	{
		SetSize(8, 26);

		nx = n;
		vx = 0;
		vy = -0.2f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_SOPHIA_VER);
		this->SetAnimationSet(ani_set);
	}
	else if (t == CANNON_BULLET_HORIZONTAL)
	{
		SetSize(8, 8);

		nx = 1;
		vx = n * 0.12f;
		vy = 0;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_RED);
		this->SetAnimationSet(ani_set);
	}
	else if (t == CANNON_BULLET_VERTICAL)
	{
		SetSize(8, 8);

		nx = 1;
		vx = 0;
		vy = n * 0.12f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_RED);
		this->SetAnimationSet(ani_set);
	}
	else if (t == FLOATER_BULLET)
	{
		SetSize(6, 6);

		nx = 1;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_FLOATER);
		this->SetAnimationSet(ani_set);
	}
	else if (t == EYEBALL_BULLET)
	{
		SetSize(8, 8);

		nx = 1;
		vx = 0.08f;
		vy = 0.08f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_RED);
		this->SetAnimationSet(ani_set);
	}
	else if (t == SKULL_BULLET)
	{
		SetSize(10, 10);

		nx = 1;
		vx = 0;
		vy = 0.15f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_BLACK);
		this->SetAnimationSet(ani_set);
	}
	else if (t == MINE_BULLET_FIRST)
	{
		startY = y;
		isFalling = false;
		SetSize(8, 8);

		nx = 1;
		vx = -0.04f;
		vy = -0.15f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_RED);
		this->SetAnimationSet(ani_set);
	}
	else if (t == MINE_BULLET_SECOND)
	{
		startY = y;
		isFalling = false;
		SetSize(8, 8);

		nx = 1;
		vx = -0.02f;
		vy = -0.15f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_RED);
		this->SetAnimationSet(ani_set);
	}
	else if (t == MINE_BULLET_THIRD)
	{
		startY = y;
		isFalling = false;
		SetSize(8, 8);

		nx = 1;
		vx = 0.02f;
		vy = -0.15f;

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_RED);
		this->SetAnimationSet(ani_set);
	}
	else if (t == MINE_BULLET_FOUTH)
	{
		startY = y;
		isFalling = false;
		SetSize(8, 8);

		nx = 1;
		vx = 0.04f;
		vy = -0.15f;

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

	if (bulletType == MINE_BULLET_FIRST || bulletType == MINE_BULLET_SECOND || bulletType == MINE_BULLET_THIRD || bulletType == MINE_BULLET_FOUTH)
	{
		if (startY - y >= 20 && !isFalling)
		{
			vy = -vy;
			isFalling = true;
		}
		x += dx;
		y += dy;
	}
	else
	{
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			if (bulletType == SKULL_BULLET)
			{
				float min_tx, min_ty, ntx, nty;

				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty);

				// block 
				x += min_tx * dx + ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
				y += min_ty * dy + nty * 0.4f;

				if (nty != 0)
				{
					vy = 0;
					vx = 0.15f;
				}
			}
			else
			{
				isDestroyed = true;
			}
		}
	}

	if (bulletType == SKULL_BULLET)
	{
		if (GetTickCount() - startFiringTime >= 2000)
			isDestroyed = true;
	}
	else
	{
		if (GetTickCount() - startFiringTime >= 1000)
			isDestroyed = true;
	}

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