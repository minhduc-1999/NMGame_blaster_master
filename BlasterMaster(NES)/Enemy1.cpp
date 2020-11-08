#include "Enemy1.h"

Enemy1::Enemy1(float x, float y) :CDynamicGameObject(x, y)
{
};

void Enemy1::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

		if (ntx != 0) vx = 0;
		if (nty != 0) vy = 0;

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Enemy1::Render()
{
	LPANIMATION ani = animations[0];
	ani->Render(x, y, 1);
}

void Enemy1::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case IDLE:
		vx = 0;
		vy = 0;
		break;
	case GO_LEFT:
		vx = -SPEED;
		vy = 0;
		nx = -1;
		break;
	case GO_RIGHT:
		vx = SPEED;
		vy = 0;
		nx = 1;
		break;
	case GO_TOP:
		vx = 0;
		vy = -SPEED;
		break;
	case GO_BOTTOM:
		vx = 0;
		vy = SPEED;
		break;
	default:
		break;
	}
}

