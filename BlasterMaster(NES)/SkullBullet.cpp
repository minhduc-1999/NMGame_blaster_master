#include "SkullBullet.h"

SkullBullet::SkullBullet(float x, float y, int team, int nx, int ny) : BaseBullet(x, y, team, nx, ny)
{
	damage = SKULL_BULLET_DAMAGE;
	SetSize(10, 10);
	SetAnimationSet(SKULL_BULLET_ANISET);
	SetState(SKULL_BULLET_STATE_FIRING);
	SetPosition(x, y + 5);

	isUpdated = false;
	isRendered = false;
}

int SkullBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (state == SKULL_BULLET_STATE_DESTROY)
		return 0;

	if (GetTickCount64() - startFiringTime >= 1500)
	{
		SetState(SKULL_BULLET_STATE_DESTROY);
		isDestroyed = true;
		isRendered = false;
		isUpdated = true;
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
			SetState(SKULL_BULLET_STATE_DESTROY);
			isDestroyed = true;
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
			if (objTeam == 0)
			{
				SetState(SKULL_BULLET_STATE_DESTROY);
				isDestroyed = true;
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
			if (e->obj->GetType() == 15 && e->ny != 0)
			{
				SetState(SKULL_BULLET_STATE_ONGROUND);
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

void SkullBullet::Render()
{
	if (isRendered)
		return;
	int ani = SKULL_BULLET_ANI_FIRING;
	if (state == SKULL_BULLET_STATE_DESTROY)
	{
		ani = SKULL_BULLET_ANI_DESTROY;
		if (!animation_set->at(SKULL_BULLET_ANI_DESTROY)->IsCompleted())
		{
			animation_set->at(SKULL_BULLET_ANI_DESTROY)->Render(x, y, nx, 255);
			isRendered = true;
			isUpdated = false;
			return;
		}
		else
		{
			animation_set->at(SKULL_BULLET_ANI_DESTROY)->ResetAnim();
			isDestroyed = true;
			isRendered = true;
			isUpdated = false;
			return;
		}
	}

	animation_set->at(ani)->Render(x, y, 1);

	isUpdated = false;
	isRendered = true;
}

void SkullBullet::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case SKULL_BULLET_STATE_FIRING:
		vx = 0.04f;
		vy = 0.3f;
		break;
	case SKULL_BULLET_STATE_ONGROUND:
		vx = 0.2f;
		vy = 0;
		break;
	case SKULL_BULLET_STATE_DESTROY:
		vx = 0;
		vy = 0;
		break;
	}
}
