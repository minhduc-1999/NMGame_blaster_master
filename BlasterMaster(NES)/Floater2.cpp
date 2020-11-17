#include "Floater2.h"

Floater2::Floater2(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(FLOATER2_WIDTH, FLOATER2_HEIGHT);
	vy = FLOATER2_FLYING_SPEED_Y;
}

void Floater2::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

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
		x += min_tx * dx + ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + nty * 0.4f;

		if (ntx != 0)
		{
			if (GetNX() == 1)
			{
				SetState(FLOATER2_STATE_FLYING_LEFT);

			}
			else
			{
				SetState(FLOATER2_STATE_FLYING_RIGHT);
			}
		}

		if (nty != 0)
		{
			vy = -vy;
		}

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Floater2::Render()
{
	int ani = FLOATER2_ANI_FLYING;

	switch (state)
	{
	case FLOATER2_STATE_FLYING_RIGHT: case FLOATER2_STATE_FLYING_LEFT:
		ani = FLOATER2_ANI_FLYING;
		break;
	case FLOATER2_STATE_DIE:
		break;
	}

	animations[ani]->Render(x, y, nx);
}

void Floater2::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case FLOATER2_STATE_FLYING_LEFT:
		vx = -FLOATER2_FLYING_SPEED_X;
		nx = -1;
		break;
	case FLOATER2_STATE_FLYING_RIGHT:
		vx = FLOATER2_FLYING_SPEED_X;
		nx = 1;
		break;
	case FLOATER2_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	}
}