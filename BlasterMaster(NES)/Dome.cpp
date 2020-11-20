#include "Dome.h"

Dome::Dome(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(DOME_WIDTH, DOME_HEIGHT);
}

void Dome::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{

		/*if (GetState() == DOME_STATE_WALKING_LEFT || GetState() == DOME_STATE_WALKING_RIGHT)
		{
			nx = -nx;
			if (GetNY() == 1)
			{
				SetState(DOME_STATE_WALKING_DOWN);
			}
			else
			{
				SetState(DOME_STATE_WALKING_UP);
			}
		}
		else if (GetState() == DOME_STATE_WALKING_UP || GetState() == DOME_STATE_WALKING_DOWN)
		{
			ny = -ny;
			if (GetNX() == 1)
			{
				SetState(DOME_STATE_WALKING_LEFT);
			}
			else
			{
				SetState(DOME_STATE_WALKING_RIGHT);
			}
		}*/

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

		if (ntx != 0)
		{
			if (GetNY() == 1)
			{
				SetState(DOME_STATE_WALKING_UP);
			}
			else
			{
				SetState(DOME_STATE_WALKING_DOWN);
			}
		}

		if (nty != 0)
		{
			if (GetNX() == 1)
			{
				SetState(DOME_STATE_WALKING_RIGHT);

			}
			else
			{
				SetState(DOME_STATE_WALKING_LEFT);
			}
		}

		//for (UINT i = 0; i < coEventsResult.size(); i++)
		//{
		//	LPCOLLISIONEVENT e = coEventsResult[i];

		//	if (dynamic_cast<Brick*>(e->obj)) // if e->obj is Goomba 
		//	{
		//		if (GetState() == DOME_STATE_WALKING_LEFT || GetState() == DOME_STATE_WALKING_RIGHT)
		//		{
		//			if (GetNY() == 1)
		//			{
		//				SetState(DOME_STATE_WALKING_DOWN);
		//			}
		//			else
		//			{
		//				SetState(DOME_STATE_WALKING_UP);
		//			}
		//		}
		//		else if (GetState() == DOME_STATE_WALKING_UP || GetState() == DOME_STATE_WALKING_DOWN)
		//		{
		//			if (GetNX() == 1)
		//			{
		//				SetState(DOME_STATE_WALKING_LEFT);
		//			}
		//			else
		//			{
		//				SetState(DOME_STATE_WALKING_RIGHT);
		//			}
		//		}
		//	}
		//}
		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Dome::Render()
{
	int ani = DOME_ANI_WALKING_LEFT_RIGHT_UP;

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
			ani = DOME_ANI_WALKING_UP_DOWN_RIGHT;
		}
		else
		{
			ani = DOME_ANI_WALKING_UP_DOWN_LEFT;
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

	//animations[ani]->Render(x, y, nx, ny);

	//if (GetNX() == 1)		// nx = 1
	//{
	//	if (GetNY() == 1)	// ny = 1
	//	{

	//	}
	//	else				// ny = -1
	//	{

	//	}
	//}
	//else					// nx = -1
	//{
	//	if (GetNY() == 1)	// ny = 1
	//	{

	//	}
	//	else				// ny = -1
	//	{

	//	}
	//}
}

void Dome::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case DOME_STATE_WALKING_LEFT:
		vx = -DOME_WALKING_SPEED;
		vy = ny * DOME_GRAVITY;
		nx = -1;
		break;
	case DOME_STATE_WALKING_RIGHT:
		vx = DOME_WALKING_SPEED;
		vy = ny * DOME_GRAVITY;
		nx = 1;
		break;
	case DOME_STATE_WALKING_UP:
		vx = nx * DOME_GRAVITY;
		vy = -DOME_WALKING_SPEED;
		ny = 1;
		break;
	case DOME_STATE_WALKING_DOWN:
		vx = nx * DOME_GRAVITY;
		vy = DOME_WALKING_SPEED;
		ny = -1;
		break;
	case DOME_STATE_DROP_DOWN:
		vx = 0;
		vy = DOME_GRAVITY;
		ny = 1;
		nx = -1;
		break;
	}
}