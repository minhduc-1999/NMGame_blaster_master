#include "Worm.h"

Worm::Worm(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(WORM_WIDTH, WORM_HEIGHT);
}

int Worm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);
	vy += WORM_GRAVITY;

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
				SetState(WORM_STATE_WALKING_LEFT);

			}
			else
			{
				SetState(WORM_STATE_WALKING_RIGHT);
			}
		}

		if (nty != 0)
		{
			vy = 0;
		}

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	return 0;
}

void Worm::Render()
{
	int ani = WORM_ANI_WALKING;
	
	switch (state)
	{
	case WORM_STATE_WALKING_LEFT: case WORM_STATE_WALKING_RIGHT:
		ani = WORM_ANI_WALKING;
		break;
	case WORM_STATE_DIE:
		break;
	}

	animation_set->at(ani)->Render(x, y, nx);
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
	case WORM_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	}
}