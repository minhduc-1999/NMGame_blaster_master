#include "Eyeball.h"

Eyeball::Eyeball(float x, float y) : CDynamicGameObject(x, y)
{
	SetSize(16, 16);
	startX = x;
	startY = y;
}

void Eyeball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += vx * dt;
		y = abs((sin((x - startY) / 15)) * startX) + startY;
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
				nx = -1;
			else
				nx = 1;
		}

		//TODO: Collision logic with dynamic object (bots)
	}
}

void Eyeball::Render()
{
	DebugOut("[INFO] start rendering eyeball\n");
	int ani = EYEBALL_ANI_ALIVE;

	if (state == EYEBALL_STATE_DIE)
	{
		ani = EYEBALL_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y, nx);
	DebugOut("[INFO] end rendering eyeball\n");

}

void Eyeball::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	nx = 1;
	vx = EYEBALL_SPEED;
}