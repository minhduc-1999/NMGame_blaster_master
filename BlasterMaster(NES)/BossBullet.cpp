#include "BossBullet.h"

BossBullet::BossBullet(float x, float y, int team, int nx, int ny) : BaseBullet(x, y, team, nx, ny)
{
	damage = BOSS_BULLET_DAMAGE;
	SetSize(9, 9);
	SetAnimationSet(BOSS_BULLET_ANISET);
	SetState(BOSS_BULLET_STATE_FIRING);
	SetPosition(x, y);

	isUpdated = false;
	isRendered = false;
}

int BossBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (state == BOSS_BULLET_STATE_DESTROY)
		return 0;

	CDynamicGameObject::Update(dt, coObjects);

	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		if (temp->GetTeam() != this->team)
		{
			SetState(BOSS_BULLET_STATE_DESTROY);
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

		//TODO: Collision logic with dynamic object
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int objTeam = e->obj->GetTeam();
			if (objTeam != this->team)
			{
				SetState(BOSS_BULLET_STATE_DESTROY);
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
	
	isUpdated = true;
	isRendered = false;

	return 0;
}

void BossBullet::Render()
{
	if (isRendered)
		return;
	animation_set->at(BOSS_BULLET_ANI_FIRING)->Render(x, y, 1);

	if (state == BOSS_BULLET_STATE_DESTROY)
		isDestroyed = true;
	isUpdated = false;
	isRendered = true;
}

void BossBullet::SetState(int state)
{
	CDynamicGameObject::SetState(state);

	switch (state)
	{
	case BOSS_BULLET_STATE_DESTROY:
		vx = 0;
		vy = 0;
		break;
	case BOSS_BULLET_STATE_FIRING:
		break;
	}
}

void BossBullet::SetSpeed(float spX, float spY)
{
	vx = spX;
	vy = spY;
}