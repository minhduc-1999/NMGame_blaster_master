#include "Orb.h"

Orb::Orb(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(18, 18);
}


int Orb::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (isDestroyed)
		return 0;

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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int coObjType = e->obj->GetType();
			if (coObjType != 15 && coObjType != 17)
			{
				if (e->nx != 0)
				{
					x += (1 - e->t) * dx - e->nx * 0.4f;
				}
				else
				{
					y += (1 - e->t) * dy - e->ny * 0.4f;
				}
			}
			switch (coObjType)
			{
			case 15: case 17:	//brick and gate
				if (e->nx != 0)
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
				break;
			};

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
	isUpdated = true;
	isRendered = false;
	return 0;
}

void Orb::Render()
{
	if (isRendered)
		return;
	int ani = ORB_ANI_ROLL;

	if (GetState() == ORB_STATE_DIE)
	{
		ani = ORB_ANI_DIE;
		if (!animation_set->at(ORB_ANI_DIE)->IsCompleted())
		{
			animation_set->at(ORB_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(ORB_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}

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
	isRendered = true;
	isUpdated = false;
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
	case ORB_STATE_DIE:
		SetSize(0, 0);
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}
}

