#include "CannonBullet.h"

CannonBullet::CannonBullet(float x, float y, int team, int nx, int ny) : BaseBullet(x, y, team, nx, ny)
{
	damage = CANNON_BULLET_DAMAGE;
	SetSize(8, 8);
	SetAnimationSet(CANNON_BULLET_ANISET);

	if (nx != 0)
	{
		if (nx == 1)
		{
			SetState(CANNON_BULLET_STATE_RIGHT);
			SetPosition(x + 7, y);
		}
		else
		{
			SetState(CANNON_BULLET_STATE_LEFT);
			SetPosition(x - 7, y);
		}

	}
	if (ny != 0)
	{
		if (ny == 1)
		{
			SetState(CANNON_BULLET_STATE_DOWN);
			SetPosition(x, y + 7);
		}
		else
		{
			SetState(CANNON_BULLET_STATE_UP);
			SetPosition(x, y - 7);
		}
	}
	isUpdated = false;
	isRendered = false;
}

int CannonBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (state == CANNON_BULLET_STATE_DESTROY)
		return 0;

	if (GetTickCount64() - startFiringTime >= 500)
	{
		SetState(CANNON_BULLET_STATE_DESTROY);
		isDestroyed = true;
		return 0;
	}

	CDynamicGameObject::Update(dt, coObjects);

	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		if (temp->GetTeam() != this->team)
		{
			SetState(CANNON_BULLET_STATE_DESTROY);
			return 0;
		}
	}
	for (UINT i = 0; i < curCoEvents.size(); i++) delete curCoEvents[i];

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
		x += min_tx * dx + ntx * 0.4f;
		y += min_ty * dy + nty * 0.4f;

		//TODO: Collision logic with dynamic object (bots)
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int objTeam = e->obj->GetTeam();
			if (objTeam != this->team)
			{
				SetState(CANNON_BULLET_STATE_DESTROY);
				break;
			}
			if (e->obj->GetType() != 15)
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
		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut("[JASON-Bullet]\tx: %f, y: %f\n", x, y);
	isUpdated = true;
	isRendered = false;

	return 0;
}

void CannonBullet::Render()
{
	if (isRendered)
		return;
	int ani = CANNON_BULLET_ANI_FIRING;
	
	if (state == CANNON_BULLET_STATE_DESTROY)
		ani = CANNON_BULLET_ANI_DESTROY;

	animation_set->at(ani)->Render(x, y, 1);

	if (state == CANNON_BULLET_STATE_DESTROY)
		isDestroyed = true;
	isUpdated = false;
	isRendered = true;
}

void CannonBullet::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case CANNON_BULLET_STATE_DESTROY:
		vx = 0;
		vy = 0;
		break;
	case CANNON_BULLET_STATE_RIGHT:
		vx = CANNON_BULLET_SPEED;
		vy = 0;
		break;
	case CANNON_BULLET_STATE_LEFT:
		vx = -CANNON_BULLET_SPEED;
		vy = 0;
		break;
	case CANNON_BULLET_STATE_UP:
		vx = 0;
		vy = -CANNON_BULLET_SPEED;
		break;
	case CANNON_BULLET_STATE_DOWN:
		vx = 0;
		vy = CANNON_BULLET_SPEED;
		break;
	}
}
