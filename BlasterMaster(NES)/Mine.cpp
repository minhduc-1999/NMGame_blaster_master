#include <cmath>
#include "Mine.h"
#include "Bullet.h"
#include "MineBullet.h"

Mine::Mine(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(15, 8);
	startTime = GetTickCount64();
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

	startTime += dt;
	if (startTime > 600)
	{
		isShooting = true;
		startTime = 0;
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
			int coObjType = e->obj->GetType();
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
			switch (coObjType)
			{
			case 15: case 17:	//brick and gate
				if (e->ny != 0)
				{
					vy = 0;
				}
				break;
			case 1: case 2:
				SetState(MINE_STATE_DIE);
				isShooting = true;
				break;
			case 20:
				if (e->obj->GetTeam() == 0)
				{
					SetState(MINE_STATE_DIE);
					isShooting = true;
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
	bullet1->SetSpeed(-0.04f, -0.15f);
	mineBulls.push_back(bullet1);
	/*MineBullet* bullet2 = new MineBullet(x, y, 1);
	bullet2->SetSpeed(-0.04f, -0.15f);
	mineBulls.push_back(bullet2);
	MineBullet* bullet3 = new MineBullet(x, y, 1);
	bullet3->SetSpeed(-0.04f, -0.15f);
	mineBulls.push_back(bullet3);
	MineBullet* bullet4 = new MineBullet(x, y, 1);
	bullet4->SetSpeed(-0.04f, -0.15f);
	mineBulls.push_back(bullet4);*/

	isShooting = false;

	return mineBulls;
}