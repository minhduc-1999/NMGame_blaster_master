#include <cmath>
#include "Mine.h"
#include "Bullet.h"

Mine::Mine(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(15, 8);
	startTime = GetTickCount();
}

int Mine::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	CDynamicGameObject::Update(dt);
	vy += MINE_GRAVITY;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (GetTickCount() - startTime >= 2000)
	{
		isShooting = true;
		isDestroyed = true;
	}

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
		x += min_tx * dx + ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + nty * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//// if e->obj is Gate 
			//if (dynamic_cast<CDynamicGameObject*>(e->obj))
			//{
			//	CDynamicGameObject* obj = dynamic_cast<CDynamicGameObject*>(e->obj);
			//	if (this->team == obj->GetTeam())
			//	{
			//		x += (1 - min_tx) * dx - ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			//		y += (1 - min_ty) * dy - nty * 0.4f;
			//		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
			//		return;
			//	}
			//}
		}

		if (nty != 0)
		{
			vy = 0;
		}

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	isUpdated = true;
	isRendered = false;
	return 0;
}

void Mine::Render()
{
	if (isRendered)
		return;
	int ani = MINE_ANI_ONGROUND;

	if (state == MINE_STATE_DIE)
	{
		ani = MINE_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y, nx);
	isRendered = true;
	isUpdated = false;
}
void Mine::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case MINE_STATE_ONGROUND:
		nx = -1;
		break;
	case MINE_STATE_DIE:
		break;
	}
}

vector<LPDYNAMICOBJECT> Mine::Fire()
{
	vector<LPDYNAMICOBJECT> mineBulls;

	mineBulls.push_back(new Bullet(x, y, MINE_BULLET_FIRST, 1));
	mineBulls.push_back(new Bullet(x, y, MINE_BULLET_SECOND, 1));
	mineBulls.push_back(new Bullet(x, y, MINE_BULLET_THIRD, 1));
	mineBulls.push_back(new Bullet(x, y, MINE_BULLET_FOUTH, 1));

	return mineBulls;
}