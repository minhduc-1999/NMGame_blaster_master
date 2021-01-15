#include "Cannon.h"
#include "CannonBullet.h"

Cannon::Cannon(float x, float y) : CDynamicGameObject(x, y)
{
	SetSize(26, 26);

	startTime = 0;
	hor = false;
}

int Cannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
		hor = !hor;
	}

	if (coEvents.size() == 0)
	{
		
	}
	else
	{
		float min_tx, min_ty, ntx, nty;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int coObjType = e->obj->GetType();
			switch (coObjType)
			{
			case 15: case 17:	//brick and gate
				if (e->ny != 0)
				{
					vy = 0;
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
	return 0;
}

void Cannon::Render()
{
	int ani = CANNON_ANI_ALIVE;

	if (state == CANNON_STATE_DIE)
	{
		ani = CANNON_ANI_DIE;
		if (!animation_set->at(CANNON_ANI_DIE)->IsCompleted())
		{
			animation_set->at(CANNON_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(CANNON_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}

	animation_set->at(ani)->Render(x, y, nx);
}

void Cannon::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	if (state == CANNON_STATE_DIE)
	{
		SetSize(0, 0);
		vx = 0;
		vy = 0;
	}
	nx = 1;
}

vector<LPDYNAMICOBJECT> Cannon::Fire()
{
	vector<LPDYNAMICOBJECT> cannonBulls;
	if (hor)
	{
		CannonBullet* bulletR = new CannonBullet(x, y, 1, 1, 0);
		CannonBullet* bulletL = new CannonBullet(x, y, 1, -1, 0);

		cannonBulls.push_back(bulletR);
		cannonBulls.push_back(bulletL);
	}
	else
	{
		CannonBullet* bulletU = new CannonBullet(x, y, 1, 0, -1);
		CannonBullet* bulletD = new CannonBullet(x, y, 1, 0, 1);

		cannonBulls.push_back(bulletU);
		cannonBulls.push_back(bulletD);
	}
	isShooting = false;

	return cannonBulls;
}