#include "Orb.h"

Orb::Orb(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(18, 18);
}


void Orb::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
				SetState(ORB_STATE_ROLLING_LEFT);

			}
			else
			{
				SetState(ORB_STATE_ROLLING_RIGHT);
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

void Orb::Render()
{
	int ani = ORB_ANI_ROLL;
	switch (state)
	{
	case ORB_STATE_ROLLING_LEFT:
		ani = ORB_ANI_ROLL;
		break;
	case ORB_STATE_ROLLING_RIGHT:
		ani = ORB_ANI_ROLL;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(x, y, nx);
}

void Orb::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case ORB_STATE_ROLLING_LEFT:
		vx = -ORB_RUNNING_SPEED;
		nx = -1;
		break;
	case ORB_STATE_ROLLING_RIGHT:
		vx = ORB_RUNNING_SPEED;
		nx = 1;
		break;
	default:
		break;
	}
}

