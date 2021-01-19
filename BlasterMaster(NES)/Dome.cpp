#include "Dome.h"
#include "Brick.h"

Dome::Dome(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(DOME_WIDTH, DOME_HEIGHT);
}

int Dome::Update(float xMain, float yMain, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

	if (!dropped)
	{
		if (abs(xMain - x) <= 40 && abs(yMain - y) <= 200)
		{
			if (xMain <= x)
			{
				nx = 1;
			}
			else
			{
				nx = -1;
			}
			SetState(DOME_STATE_DROP_DOWN);
		}
	}

	SetBottomRect(RectType);

	isCollision = false;
	if (coObjects != NULL)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->GetType() != 5)
			{
				if (CheckIfBound(bottomRect, coObjects->at(i)->GetBound()))
				{
					isCollision = true;
					break;
				}
			}
		}
	}
	if (!isCollision && GetState() != DOME_STATE_DROP_DOWN)
	{
		if (GetState() == DOME_STATE_WALKING_LEFT || GetState() == DOME_STATE_WALKING_RIGHT)
		{
			nx = -nx;
			if (GetNY() == 1)
			{
				SetState(DOME_STATE_WALKING_DOWN);
				y += 1;
			}
			else
			{
				SetState(DOME_STATE_WALKING_UP);
				y -= 1;
			}
		}
		else if (GetState() == DOME_STATE_WALKING_UP || GetState() == DOME_STATE_WALKING_DOWN)
		{
			ny = -ny;
			if (GetNX() == 1)
			{
				SetState(DOME_STATE_WALKING_RIGHT);
				x += 2;
			}
			else
			{
				SetState(DOME_STATE_WALKING_LEFT);
				x -= 2;
			}
		}
		x += dx;
		y += dy;
	}
	else
	{
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
				if (ny == -1)
				{
					SetState(DOME_STATE_WALKING_DOWN);
				}
				else
				{
					SetState(DOME_STATE_WALKING_UP);
				}
			}
			else
				x += dx;
			//x += min_tx * dx + ntx * 0.4f;
			if (nby != 0)
			{
				y += min_tby * dy + nby * 0.4f;
				if (nx == 1)
				{
					SetState(DOME_STATE_WALKING_LEFT);
				}
				else
				{
					SetState(DOME_STATE_WALKING_RIGHT);
				}
			}
			else
				y += dy;
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	isUpdated = true;
	isRendered = false;
	// clean up collision events
	return 0;
}

void Dome::Render()
{
	if (isRendered)
	{
		return;
	}
	int ani = DOME_ANI_WALKING_LEFT_RIGHT_UP;

	if (GetState() == DOME_STATE_DIE)
	{
		ani = DOME_ANI_DIE;
		if (!animation_set->at(DOME_ANI_DIE)->IsCompleted())
		{
			animation_set->at(DOME_ANI_DIE)->Render(x, y, nx, 255);
			isRendered = true;
			isUpdated = false;
			return;
		}
		else
		{
			animation_set->at(DOME_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			isRendered = true;
			isUpdated = false;
			return;
		}
	}

	switch (state)
	{
	case DOME_STATE_WALKING_LEFT:case DOME_STATE_WALKING_RIGHT:
		if (GetNY() == 1)
		{
			ani = DOME_ANI_WALKING_LEFT_RIGHT_UP;
		}
		else
		{
			ani = DOME_ANI_WALKING_LEFT_RIGHT_DOWN;
		}
		animation_set->at(ani)->Render(x, y, nx);
		isRendered = true;
		isUpdated = false;
		return;
		break;
	case DOME_STATE_WALKING_UP:case DOME_STATE_WALKING_DOWN:
		if (GetNX() == 1)
		{
			ani = DOME_ANI_WALKING_UP_DOWN_LEFT;
		}
		else
		{
			ani = DOME_ANI_WALKING_UP_DOWN_RIGHT;
		}
		animation_set->at(ani)->Render(x, y, -1);
		isRendered = true;
		isUpdated = false;
		return;
		break;
	case DOME_STATE_DROP_DOWN:
		ani = DOME_ANI_WALKING_LEFT_RIGHT_DOWN;
		animation_set->at(ani)->Render(x, y, nx);
		isRendered = true;
		isUpdated = false;
		return;
		break;
	}
}

void Dome::SetBottomRect(int rectPos)
{
	bottomRect = GetBound();
	dropped = false;
	switch (rectPos)
	{
	case RECT_LEFT:
		RectType = RECT_LEFT;
		bottomRect.left -= 5;
		bottomRect.right -= 5;
		break;
	case RECT_RIGHT:
		RectType = RECT_RIGHT;
		bottomRect.left += 5;
		bottomRect.right += 5;
		break;
	case RECT_TOP:
		RectType = RECT_TOP;
		bottomRect.top -= 5;
		bottomRect.bottom -= 5;
		break;
	case RECT_BOTTOM:
		dropped = true;
		RectType = RECT_BOTTOM;
		
		bottomRect.top += 5;
		bottomRect.bottom += 5;
		break;
	default:
		break;
	}
}

void Dome::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case DOME_STATE_WALKING_LEFT:
		if (ny == 1)
		{
			SetBottomRect(RECT_BOTTOM);
		}
		else
		{
			SetBottomRect(RECT_TOP);
		}
		vx = -DOME_WALKING_SPEED;
		vy = 0;
		nx = -1;
		break;
	case DOME_STATE_WALKING_RIGHT:
		if (ny == 1)
		{
			SetBottomRect(RECT_BOTTOM);
		}
		else
		{
			SetBottomRect(RECT_TOP);
		}
		vx = DOME_WALKING_SPEED;
		vy = 0;
		nx = 1;
		break;
	case DOME_STATE_WALKING_UP:
		if (nx == 1)
		{
			SetBottomRect(RECT_RIGHT);
		}
		else
		{
			SetBottomRect(RECT_LEFT);
		}
		vx = 0;
		vy = -DOME_WALKING_SPEED;
		ny = -1;
		break;
	case DOME_STATE_WALKING_DOWN:
		if (nx == 1)
		{
			SetBottomRect(RECT_RIGHT);
		}
		else
		{
			SetBottomRect(RECT_LEFT);
		}
		vx = 0;
		vy = DOME_WALKING_SPEED;
		ny = 1;
		break;
	case DOME_STATE_DROP_DOWN:
		dropped = true;
		SetBottomRect(RECT_TOP);
		vx = 0;
		vy = DOME_GRAVITY;
		ny = 1;
		break;
	case DOME_STATE_GO_UP:
		dropped = false;
		SetBottomRect(RECT_BOTTOM);
		vx = 0;
		vy = -DOME_GRAVITY;
		ny = -1;
		nx = 1;
		break;
	case DOME_STATE_DIE:
		SetSize(0, 0);
		vx = 0;
		vy = 0;
		break;
	}
}