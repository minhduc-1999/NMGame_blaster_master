#include "Worm.h"

Worm::Worm(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(WORM_WIDTH, WORM_HEIGHT);
}

void Worm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);
	vy += WORM_GRAVITY;

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

		x += min_tx * dx + ntx * 0.4f;
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
						SetState(WORM_STATE_WALKING_LEFT);

					}
					else
					{
						SetState(WORM_STATE_WALKING_RIGHT);
					}
				}
				if (e->ny != 0)
				{
					vy = 0;
				}
				break;
			case 20: //enemy bullet
				if (e->obj->GetTeam() == 0)
				{
					SetState(WORM_STATE_DIE);
				}
				break;
			default:
				break;
			};

		}
		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Worm::Render()
{
	int ani = WORM_ANI_WALKING;

	if (GetState() == WORM_STATE_DIE)
	{
		ani = WORM_ANI_DIE;
		if (!animation_set->at(WORM_ANI_DIE)->IsCompleted())
		{
			animation_set->at(WORM_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(WORM_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}
	
	switch (state)
	{
	case WORM_STATE_WALKING_LEFT: case WORM_STATE_WALKING_RIGHT:
		ani = WORM_ANI_WALKING;
		break;
	case WORM_STATE_DIE:
		break;
	}

	animation_set->at(ani)->Render(x, y, nx);
}

void Worm::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case WORM_STATE_WALKING_LEFT:
		vx = -WORM_WALKING_SPEED;
		nx = -1;
		break;
	case WORM_STATE_WALKING_RIGHT:
		vx = WORM_WALKING_SPEED;
		nx = 1;
		break;
	case WORM_STATE_DIE:
		SetSize(0, 0);
		vx = 0;
		vy = 0;
		break;
	}
}