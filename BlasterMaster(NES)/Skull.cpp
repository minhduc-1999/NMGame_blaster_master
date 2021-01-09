#include "Skull.h"
#include <cmath>

Skull::Skull(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(18, 21);
	startX = x;
	startY = y;
	vx = -SKULL_FLYING_SPEED;
	vy = 0;
	startTime = 0;
}


int Skull::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	CDynamicGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	startTime += dt;

	if (startTime > 600)
	{
		isShooting = true;
		startTime = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		if (startX - x > 100)
		{
			vx = 0;
			vy = -SKULL_FLYING_SPEED;
			SetState(SKULL_STATE_FLYING_RIGHT);
		}
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
			case 20: //enemy bullet
				if (e->obj->GetTeam() == 0)
				{
					SetState(SKULL_STATE_DIE);
				}
				break;
			default:
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

	isRendered = false;
	isUpdated = true;
	return 0;
}

void Skull::Render()
{
	if (isRendered)
		return;
	int ani = SKULL_ANI_FLYING;

	if (GetState() == SKULL_STATE_DIE)
	{
		ani = SKULL_ANI_DIE;
		if (!animation_set->at(SKULL_ANI_DIE)->IsCompleted())
		{
			animation_set->at(SKULL_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(SKULL_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}

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

	isRendered = true;
	isUpdated = false;
}
void Skull::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case SKULL_STATE_FLYING_LEFT:
		nx = -1;
		break;
	case SKULL_STATE_FIRE_LEFT:
		nx = -1;
		break;
	case SKULL_STATE_FLYING_RIGHT:
		nx = 1;
		break;
	case SKULL_STATE_FIRE_RIGHT:
		nx = 1;
		break;
	case SKULL_STATE_DIE:
		SetSize(0, 0);
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}
}

vector<LPDYNAMICOBJECT> Skull::Fire()
{
	vector<LPDYNAMICOBJECT> skullBulls;

	Bullet* bullet = new Bullet(x, y, SKULL_BULLET, 1);
	skullBulls.push_back(bullet);

	return skullBulls;
}
