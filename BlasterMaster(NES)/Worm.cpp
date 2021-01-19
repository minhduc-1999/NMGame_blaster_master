#include "Worm.h"

Worm::Worm(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(WORM_WIDTH, WORM_HEIGHT);
}

int Worm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
	{
		return -1;
	}
	if (isDestroyed)
	{
		return 0;
	}
	CDynamicGameObject::Update(dt);

	//check now collision
	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	isCollisionWithMagma = false;
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		switch (temp->GetType())
		{
		case 19:
			isCollisionWithMagma = true;
			break;
		default:
			break;
		}
	}
	for (UINT i = 0; i < curCoEvents.size(); i++) delete curCoEvents[i];

	if (state != WORM_STATE_GO_UP)
	{
		vy += WORM_GRAVITY;
	}
	else
	{
		vy = -WORM_GO_UP_SPEED;
	}

	if (!isCollisionWithMagma)
	{
		y -= 0.04f;
		if (GetNX() == -1)
		{
			x -= 0.04f;
			SetState(WORM_STATE_WALKING_LEFT);

		}
		else
		{
			x += 0.04f;
			SetState(WORM_STATE_WALKING_RIGHT);
		}
	}

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
		float min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby);

		if (nbx != 0)
		{
			x += min_tbx * dx + nbx * 0.4f;
			if (isCollisionWithMagma)
			{
				SetState(WORM_STATE_GO_UP);
			}
			else if (nbx == -1)
			{
				SetState(WORM_STATE_WALKING_LEFT);

			}
			else
			{
				SetState(WORM_STATE_WALKING_RIGHT);
			}
		}
		else
			x += dx;
		//x += min_tx * dx + ntx * 0.4f;
		if (nby != 0)
		{
			y += min_tby * dy + nby * 0.4f;
			vy = 0;
		}
		else
			y += dy;

		/*for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int coObjType = e->obj->GetType();
			switch (coObjType)
			{
			case 20: //enemy bullet
				break;
			default:
				break;
			};

		}*/

		
		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	isUpdated = true;
	isRendered = false;
	return 0;
}

void Worm::Render()
{
	if (isRendered)
	{
		return;
	}
	int ani = WORM_ANI_WALKING;

	if (GetState() == WORM_STATE_DIE)
	{
		ani = WORM_ANI_DIE;
		if (!animation_set->at(WORM_ANI_DIE)->IsCompleted())
		{
			animation_set->at(WORM_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(WORM_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}

	animation_set->at(ani)->Render(x, y, nx);
	isRendered = true;
	isUpdated = false;
}

void Worm::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case WORM_STATE_WALKING_LEFT:
		vx = -WORM_WALKING_SPEED;
		nx = -1;
		break;
	case WORM_STATE_WALKING_RIGHT:
		vx = WORM_WALKING_SPEED;
		nx = 1;
		break;
	case WORM_STATE_GO_UP:
		vx = 0;
		vy = -WORM_GO_UP_SPEED;
		break;
	case WORM_STATE_DIE:
		SetSize(0, 0);
		vx = 0;
		vy = 0;
		break;
	}
}