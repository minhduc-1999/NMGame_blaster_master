#include "Skull.h"
#include <cmath>

Skull::Skull(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(18, 21);
}


void Skull::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, ntx, nty;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty);

		// block 
		x += min_tx * dx + ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + nty * 0.4f;

		if (ntx != 0)
		{
			if (GetNX() == 1)
			{
				SetState(SKULL_STATE_FIRE_LEFT);

			}
			else
			{
				SetState(SKULL_STATE_FIRE_RIGHT);
			}
		}

	/*	if (nty != 0)
		{
			if (GetNX() == 1)
			{
				SetState(SKULL_STATE_FLYING_LEFT);
				vy = 0;
			}
			else
			{
				SetState(SKULL_STATE_FLYING_RIGHT);
				vy = 0;
			}
		}*/

		//TODO: Collision logic with dynamic object (bots)
	}
}

void Skull::Render()
{
	int ani = SKULL_ANI_FLYING;
	switch (state)
	{
	case SKULL_STATE_FLYING_LEFT:
		ani = SKULL_ANI_FLYING;
		break;
	case SKULL_STATE_FLYING_RIGHT:
		ani = SKULL_ANI_FLYING;
		break;
	case SKULL_STATE_FIRE_LEFT:
		ani = SKULL_ANI_FIRE;
		break;
	case SKULL_STATE_FIRE_RIGHT:
		ani = SKULL_ANI_FIRE;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(x, y, nx);
}
void Skull::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case SKULL_STATE_FLYING_LEFT:
		vx = -SKULL_FLYING_SPEED;
		nx = -1;
		break;
	case SKULL_STATE_FIRE_LEFT:
		vx = 0;
		vy = -SKULL_FLYING_SPEED;
		nx = -1;
		break;
	case SKULL_STATE_FLYING_RIGHT:
		vx = SKULL_FLYING_SPEED;
		nx = 1;
		break;
	case SKULL_STATE_FIRE_RIGHT:
		vx = 0;
		vy = -SKULL_FLYING_SPEED;
		nx = 1;
		break;
	default:
		break;
	}
}
