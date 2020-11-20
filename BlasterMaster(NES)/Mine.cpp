#include <cmath>
#include "Mine.h"

Mine::Mine(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(15, 8);
}

void Mine::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);
	vy += MINE_GRAVITY;
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


		if (nty != 0)
		{
			vy = 0;
		}

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void Mine::Render()
{
	int ani = MINE_ANI_ONGROUND;

	if (state == MINE_STATE_DIE)
	{
		ani = MINE_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y, nx);
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