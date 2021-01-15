#include "MineBullet.h"

MineBullet::MineBullet(float x, float y, int team, int nx, int ny) : BaseBullet(x, y, team, nx, ny)
{
	startY = y;
	isFalling = false;

	damage = MINE_BULLET_DAMAGE;
	SetSize(8, 8);
	SetAnimationSet(MINE_BULLET_ANISET);
	SetState(MINE_BULLET_STATE_FIRING);
	SetPosition(x, y - 3);
	vx = -0.04f;
	vy = -0.15f;

	isUpdated = false;
	isRendered = false;
}

int MineBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (state == MINE_BULLET_STATE_DESTROY)
		return 0;

	if (GetTickCount64() - startFiringTime >= 1000)
	{
		SetState(MINE_BULLET_STATE_DESTROY);
		return 0;
	}

	CDynamicGameObject::Update(dt);

	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		if (temp->GetType() == 1 || temp->GetType() == 2)
		{
			SetState(MINE_BULLET_STATE_DESTROY);
			return 0;
		}
	}
	for (UINT i = 0; i < curCoEvents.size(); i++) delete curCoEvents[i];

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (startY - y >= 20 && !isFalling)
	{
		vy = -vy;
		isFalling = true;
	}
	x += dx;
	y += dy;

	 //No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		
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
			int coObjType = e->obj->GetType();
			if (e->obj->GetType() == 1 || e->obj->GetType() == 2)
			{
				SetState(MINE_BULLET_STATE_DESTROY);
				break;
			}
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
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	DebugOut("[JASON-Bullet]\tx: %f, y: %f\n", x, y);
	isUpdated = true;
	isRendered = false;

	return 0;
}

void MineBullet::Render()
{
	if (isRendered)
		return;
	int ani = MINE_BULLET_ANI_FIRING;
	switch (state)
	{
	case MINE_BULLET_STATE_DESTROY:
		ani = MINE_BULLET_ANI_DESTROY;
		break;
	case MINE_BULLET_STATE_FIRING:
		break;
	}
	animation_set->at(ani)->Render(x, y, 1);
	if (state == MINE_BULLET_STATE_DESTROY)
		isDestroyed = true;
	isUpdated = false;
	isRendered = true;
}

void MineBullet::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case MINE_BULLET_STATE_DESTROY:
		vx = 0;
		vy = 0;
		break;
	case MINE_BULLET_STATE_FIRING:
		break;
	}
}

void MineBullet::SetSpeed(float spX, float spY)
{
	vx = spX;
	vy = spY;
}
