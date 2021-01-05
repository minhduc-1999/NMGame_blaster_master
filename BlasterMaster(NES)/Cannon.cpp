#include "Cannon.h"

Cannon::Cannon(float x, float y) : CDynamicGameObject(x, y)
{
	SetSize(26, 26);

	startTime = 0;
	hor = false;
}

void Cannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	startTime += dt;

	if (startTime > 600)
	{
		isShooting = true;
		startTime = 0;
		hor = !hor;
	}

	if (coEvents.size() == 0)
	{
		
	}
	else
	{
		float min_tx, min_ty, ntx, nty;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty);

		// block 
		//x += min_tx * dx + ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + nty * 0.4f;


		if (nty != 0)
		{
			vy = 0;
		}

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Cannon::Render()
{
	int ani = CANNON_ANI_ALIVE;

	if (state == CANNON_STATE_DIE)
	{
		ani = CANNON_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y, nx);
}

void Cannon::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	nx = 1;
}

vector<LPDYNAMICOBJECT> Cannon::Fire()
{
	vector<LPDYNAMICOBJECT> cannonBulls;
	if (hor)
	{
		Bullet* bulletR = new Bullet(x + 7, y, CANNON_BULLET_HORIZONTAL, 1);
		Bullet* bulletL = new Bullet(x - 7, y, CANNON_BULLET_HORIZONTAL, -1);

		cannonBulls.push_back(bulletR);
		cannonBulls.push_back(bulletL);
	}
	else
	{
		Bullet* bulletU = new Bullet(x, y - 7, CANNON_BULLET_VERTICAL, -1);
		Bullet* bulletD = new Bullet(x, y + 7, CANNON_BULLET_VERTICAL, 1);

		cannonBulls.push_back(bulletU);
		cannonBulls.push_back(bulletD);
	}

	return cannonBulls;
}