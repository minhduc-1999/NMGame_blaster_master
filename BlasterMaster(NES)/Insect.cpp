#include "Insect.h"
Insect::Insect(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(18,	18);
	vy = 0.03f;
	vx = 0.03f;
	nx = 1;
	startY = y;
}
int Insect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (isDestroyed)
		return 0;

	CDynamicGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		if (y > startY + 20)
		{
			vx = nx * 0.03f;
			vy = -0.1f;
		}
		else if (y < startY)
		{
			vx = nx * 0.03f;
			vy = 0.03f;
		}
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
				if (e->nx != 0)
				{
					/*if (GetNX() == 1)
					{
						nx = ;
					}
					else
					{
						nx = -1;
					}*/
					nx = -nx;
					vx = -vx;
				}
				if (e->ny != 0)
				{
					/*vy = -INSECT_FLYING_SPEED * 10;
					vx = INSECT_FLYING_SPEED * 5;*/
					vy = -vy;
				}
				break;
			};

		}

		//TODO: Collision logic with dynamic object (bots)
	}
	isUpdated = true;
	isRendered = false;
	return 0;
}

void Insect::Render()
{
	if (isRendered)
		return;
	int ani = INSECT_ANI_FLYING;

	if (GetState() == INSECT_STATE_DIE)
	{
		ani = INSECT_ANI_DIE;
		if (!animation_set->at(INSECT_ANI_DIE)->IsCompleted())
		{
			animation_set->at(INSECT_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(INSECT_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}
	
	animation_set->at(ani)->Render(x, y, nx);
	isRendered = true;
	isUpdated = false;
}

void Insect::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	if (state == INSECT_STATE_DIE)
	{
		SetSize(0, 0);
		vx = 0;
		vy = 0;
	}
}

