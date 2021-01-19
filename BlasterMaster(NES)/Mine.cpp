#include <cmath>
#include "Mine.h"
#include "MineBullet.h"

Mine::Mine(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(15, 8);
}

int Mine::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	CDynamicGameObject::Update(dt);

	vy += MINE_GRAVITY;

	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		int objTeam = temp->GetTeam();
		if (objTeam == 0)
		{
			SetState(MINE_BULLET_STATE_DESTROY);
			//isDestroyed = true;
			isShooting = true;
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

	/*if (isDestroyed == true)
	{
		isShooting = true;
	}*/

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby);

		// block 
		if (nbx != 0)
			x += min_tbx * dx + nbx * 0.4f;
		else
			x += dx;
		if (nby != 0)
			y += min_tby * dy + nby * 0.4f;
		else
			y += dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int coObjType = e->obj->GetType();
			switch (coObjType)
			{
			case 1: case 2:
				SetState(MINE_STATE_DIE);
				isShooting = true;
				break;
			default:
				break;
			};

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
		if (!animation_set->at(MINE_ANI_DIE)->IsCompleted())
		{
			animation_set->at(MINE_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(MINE_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
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
		SetSize(0, 0);
		vx = 0;
		vy = 0;
		break;
	}
}

vector<LPDYNAMICOBJECT> Mine::Fire()
{
	vector<LPDYNAMICOBJECT> mineBulls;

	MineBullet* bullet1 = new MineBullet(x, y, 1);
	bullet1->SetSpeed(-0.04f, -0.17f);
	mineBulls.push_back(bullet1);
	MineBullet* bullet2 = new MineBullet(x, y, 1);
	bullet2->SetSpeed(-0.02f, -0.17f);
	mineBulls.push_back(bullet2);
	MineBullet* bullet3 = new MineBullet(x, y, 1);
	bullet3->SetSpeed(0.02f, -0.17f);
	mineBulls.push_back(bullet3);
	MineBullet* bullet4 = new MineBullet(x, y, 1);
	bullet4->SetSpeed(0.04f, -0.17f);
	mineBulls.push_back(bullet4);
	Sound::getInstance()->play("Mine", false, 1);

	isShooting = false;

	return mineBulls;
}