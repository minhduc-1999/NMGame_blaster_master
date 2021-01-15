#include "Dome.h"
#include "Brick.h"

Dome::Dome(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(DOME_WIDTH, DOME_HEIGHT);
}

int Dome::Update(float xMain, float yMain, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	if (!dropped)
	{
		if (abs(xMain - x) <= 40 && abs(yMain - y) <= 200)
		{
			SetState(DOME_STATE_DROP_DOWN);
		}
	}

	SetBottomRect(RectType);

	bool isCollision = false;
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
			float min_tx, min_ty, ntx, nty;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty);

			x += min_tx * dx + ntx * 0.4f;
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
						if (ny == -1)
						{
							SetState(DOME_STATE_WALKING_DOWN);
						}
						else
						{
							SetState(DOME_STATE_WALKING_UP);
						}
					}
					if (e->ny != 0)
					{
						if (nx == 1)
						{
							SetState(DOME_STATE_WALKING_LEFT);
						}
						else
						{
							SetState(DOME_STATE_WALKING_RIGHT);
						}
					}
					break;
				case 20: //enemy bullet
					if (e->obj->GetTeam() == 0)
					{
						SetState(DOME_STATE_DIE);
					}
					break;
				default:
					break;
				};

			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

	// clean up collision events
	return 0;
}

void Dome::Render()
{
	int ani = DOME_ANI_WALKING_LEFT_RIGHT_UP;

	if (GetState() == DOME_STATE_DIE)
	{
		ani = DOME_ANI_DIE;
		if (!animation_set->at(DOME_ANI_DIE)->IsCompleted())
		{
			animation_set->at(DOME_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(DOME_ANI_DIE)->ResetAnim();
			isDestroyed = true;
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
		return;
		break;
	case DOME_STATE_DROP_DOWN:
		ani = DOME_ANI_WALKING_LEFT_RIGHT_DOWN;
		animation_set->at(ani)->Render(x, y, nx);
		return;
		break;
	}
}

void Dome::SetBottomRect(int rectPos)
{
	bottomRect = GetBound();
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
		nx = 1;
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