#include "Rock.h"

Rock::Rock(float x, float y) : CDynamicGameObject(x, y)
{
	SetSize(16, 16);
	vx = vy = 0;
	isUpdated = false;
	isRendered = false;
}

int Rock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (isDestroyed)
		return 0;

	CDynamicGameObject::Update(dt);

	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		int objTeam = temp->GetTeam();
		if (objTeam != this->team)
		{
			SetState(ROCK_STATE_DESTROY);
			isDestroyed = true;
			isUpdated = true;
			isRendered = false;
			return 0;
		}
	}
	for (UINT i = 0; i < curCoEvents.size(); i++) delete curCoEvents[i];

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

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
			if (e->obj->GetType() == 20 && e->obj->GetTeam() == 0)
			{
				SetState(ROCK_STATE_DESTROY);
				isDestroyed = true;
				continue;
			}
		}

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	isUpdated = true;
	isRendered = false;
	return 0;
}

void Rock::Render()
{
	int ani = ROCK_ANI_ONMAP;

	if (state == ROCK_STATE_DESTROY)
	{
		ani = ROCK_ANI_DESTROY;
		if (!animation_set->at(ROCK_ANI_DESTROY)->IsCompleted())
		{
			animation_set->at(ROCK_ANI_DESTROY)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(ROCK_ANI_DESTROY)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}

	animation_set->at(ani)->Render(x, y, nx);
}

void Rock::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	if (state == ROCK_STATE_DESTROY)
	{
		SetSize(0, 0);
		vx = 0;
		vy = 0;
	}
	nx = 1;
}
