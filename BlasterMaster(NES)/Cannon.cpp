#include "Cannon.h"
#include "Bullet.h"

Cannon::Cannon(float x, float y) : CDynamicGameObject(x, y)
{
	SetSize(26, 26);
}

void Cannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		int dft = GetTickCount() % 1000;

		if (dft > 250 && dft < 500)
		{
			SetState(CANNON_STATE_FIRE_VER);
		}
		else if (dft > 750 && dft < 999)
		{
			SetState(CANNON_STATE_FIRE_HOR);
		}
		else
		{
			SetState(CANNON_STATE_ALIVE);
		}

		if (GetState() == CANNON_STATE_FIRE_VER)
		{

		}
		else if (GetState() == CANNON_STATE_FIRE_HOR)
		{

		}
	}
	else
	{
		float min_tx, min_ty, ntx, nty;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty);

		// block 
		x += min_tx * dx + ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + nty * 0.4f;


		if (nty != 0)
		{
			vy = 0;
		}

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Cannon::Render()
{
	int ani = CANNON_ANI_ALIVE;

	if (state == CANNON_STATE_DIE)
	{
		ani = CANNON_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y, nx);
}

void Cannon::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	nx = 1;
}