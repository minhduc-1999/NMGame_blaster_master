#include "SophiaBullet.h"

SophiaBullet::SophiaBullet(float x, float y, int team, int t, int nx, int ny) : BaseBullet(x, y, team, nx, ny)
{
	damage = SOPHIA_BULLET_DAMAGE;
	type = t;
	if (type == 0)
	{
		SetAnimationSet(SOPHIA_BULLET_ANISET);
		if (nx != 0)
			SetSize(8, 26);
		if (ny != 0)
			SetSize(26, 8);
	}
	else if (type == 1)
	{
		SetAnimationSet(SOPHIA_BULLET_ROCKET_ANISET);
		SetSize(19, 8);
	}

	if (nx != 0)
	{
		if (nx == 1)
		{
			SetState(SOPHIA_BULLET_STATE_RIGHT);
			SetPosition(x + 5, y - 5);
		}
		else
		{
			SetState(SOPHIA_BULLET_STATE_LEFT);
			SetPosition(x - 5, y - 5);
		}

	}
	if (ny != 0)
	{
		SetState(SOPHIA_BULLET_STATE_UP);
		if (nx == 1)
		{
			SetPosition(x - 4, y - 20);
		}
		else
		{
			SetPosition(x + 4, y - 20);
		}
	}
	isUpdated = false;
	isRendered = false;
}

int SophiaBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (state == SOPHIA_BULLET_STATE_DESTROY)
		return 0;

	if (GetTickCount64() - startFiringTime >= 200 && type == 1)
	{
		if (vy != 0)
			vy = 0;
	}

	if (GetTickCount64() - startFiringTime >= 500)
	{
		SetState(SOPHIA_BULLET_STATE_DESTROY);
		isDestroyed = true;
		return 0;
	}

	CDynamicGameObject::Update(dt, coObjects);

	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		int objTeam = temp->GetTeam();
		if (objTeam == 1)
		{
			LPDYNAMICOBJECT dyn = dynamic_cast<LPDYNAMICOBJECT>(temp);
			dyn->SetIsDestroyed();
			SetState(SOPHIA_BULLET_STATE_DESTROY);
			return 0;
		}
		else if (objTeam == -1)
		{
			if (type == 0)
			{
				SetState(SOPHIA_BULLET_STATE_DESTROY);
				return 0;
			}
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
			if (objTeam == 1)
			{
				LPDYNAMICOBJECT dyn = dynamic_cast<LPDYNAMICOBJECT>(e->obj);
				dyn->SetIsDestroyed();
				SetState(SOPHIA_BULLET_STATE_DESTROY);
				return 0;
			}
			else if (objTeam == -1)
			{
				if (type == 0)
				{
					SetState(SOPHIA_BULLET_STATE_DESTROY);
					break;
				}
			}
			if (e->obj->GetType() != 15 || (e->obj->GetType() == 15 && type == 1))
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

void SophiaBullet::Render()
{
	if (isRendered)
		return;
	int dir = 1;
	int ani = SOPHIA_BULLET_ANI_HORIZONTAL;
	switch (state)
	{
	case SOPHIA_BULLET_STATE_DESTROY:
		ani = SOPHIA_BULLET_ANI_DESTROY;
		break;
	case SOPHIA_BULLET_STATE_LEFT:
		break;
	case SOPHIA_BULLET_STATE_RIGHT:
		dir = -1;
		break;
	case SOPHIA_BULLET_STATE_UP:
		ani = SOPHIA_BULLET_ANI_VERTICAL;
		break;
	}
	animation_set->at(ani)->Render(x, y, dir);
	if (state == SOPHIA_BULLET_STATE_DESTROY)
		isDestroyed = true;
	isUpdated = false;
	isRendered = true;
}

void SophiaBullet::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case SOPHIA_BULLET_STATE_DESTROY:
		vx = 0;
		vy = 0;
		break;
	case SOPHIA_BULLET_STATE_RIGHT:
		vx = SOPHIA_BULLET_SPEED;
		vy = 0;
		break;
	case SOPHIA_BULLET_STATE_LEFT:
		vx = -SOPHIA_BULLET_SPEED;
		vy = 0;
		break;
	case SOPHIA_BULLET_STATE_UP:
		vx = 0;
		vy = -SOPHIA_BULLET_SPEED;
		break;
	}
}

void SophiaBullet::SetSpeed(float spX, float spY)
{
	vx = spX;
	vy = spY;
}
