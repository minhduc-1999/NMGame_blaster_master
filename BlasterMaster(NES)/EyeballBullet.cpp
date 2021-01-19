#include "EyeballBullet.h"

EyeballBullet::EyeballBullet(float x, float y, int team, int nx, int ny) : BaseBullet(x, y, team, nx, ny)
{
	damage = EYEBALL_BULLET_DAMAGE;
	SetSize(8, 8);
	SetAnimationSet(EYEBALL_BULLET_ANISET);
	SetState(EYEBALL_BULLET_STATE_FIRING);
	SetPosition(x + 7, y);
	
	isUpdated = false;
	isRendered = false;
}

int EyeballBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (state == EYEBALL_BULLET_STATE_DESTROY)
		return 0;

	if (GetTickCount64() - startFiringTime >= 800)
	{
		SetState(EYEBALL_BULLET_STATE_DESTROY);
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
			SetState(EYEBALL_BULLET_STATE_DESTROY);
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
			if (objTeam != this->team)
			{
				SetState(EYEBALL_BULLET_STATE_DESTROY);
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
		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut("[JASON-Bullet]\tx: %f, y: %f\n", x, y);
	isUpdated = true;
	isRendered = false;

	return 0;
}

void EyeballBullet::Render()
{
	if (isRendered)
		return;
	int ani = EYEBALL_BULLET_ANI_FIRING;
	switch (state)
	{
	case EYEBALL_BULLET_STATE_DESTROY:
		ani = EYEBALL_BULLET_ANI_DESTROY;
		break;
	case EYEBALL_BULLET_STATE_FIRING:
		break;
	}
	animation_set->at(ani)->Render(x, y, 1);
	if (state == EYEBALL_BULLET_STATE_DESTROY)
		isDestroyed = true;
	isUpdated = false;
	isRendered = true;
}

void EyeballBullet::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case EYEBALL_BULLET_STATE_DESTROY:
		vx = 0;
		vy = 0;
		break;
	case EYEBALL_BULLET_STATE_FIRING:
		break;
	}
}

void EyeballBullet::SetSpeed(float spX, float spY)
{
	vx = spX;
	vy = spY;
}
