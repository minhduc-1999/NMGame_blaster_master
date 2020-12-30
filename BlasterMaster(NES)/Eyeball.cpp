#include "Eyeball.h"

Eyeball::Eyeball(float x, float y) : CDynamicGameObject(x, y)
{
	SetSize(16, 16);
	startX = x;
	startY = y;
	vx = 0.01f;
	vy = 0.06f;
	startTime = 0;
}

void Eyeball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	startTime += dt;

	if (startTime > 800)
	{
		Bullet* bullet = new Bullet(x + 7, y, EYEBALL_BULLET, 1);
		eyeballBulls.push_back(bullet);
		startTime = 0;
	}

	for (int i = 0; i < eyeballBulls.size(); i++)
		eyeballBulls[i]->Update(dt, coObjects);

	if (coEvents.size() == 0)
	{
		if (y - startY > 50 || y <= startY)
		{
			vy = -vy;
			x += vx * dt;
			y += vy * dt;
		}
		else
		{
			x += dx;
			y += dy;
		}
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
			vx = -vx;
		}
		if (nty != 0)
		{
			vy = -vy;
		}

		//TODO: Collision logic with dynamic object (bots)
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Eyeball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, float xMain, float yMain)
{
	CDynamicGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	startTime += dt;

	if (startTime > 800)
	{
		Bullet* bullet = new Bullet(x + 7, y, EYEBALL_BULLET, 1);
		float a = xMain - x;
		float b = yMain - y;
		bullet->SetSpeed(a / sqrt(pow(a, 2) + pow(b, 2)) / 5, b / sqrt(pow(a, 2) + pow(b, 2)) / 5);
		eyeballBulls.push_back(bullet);
		startTime = 0;
	}

	for (int i = 0; i < eyeballBulls.size(); i++)
		eyeballBulls[i]->Update(dt, coObjects);

	if (coEvents.size() == 0)
	{
		if (y - startY > 50 || y <= startY)
		{
			vy = -vy;
			x += vx * dt;
			y += vy * dt;
		}
		else
		{
			x += dx;
			y += dy;
		}
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
			vx = -vx;
		}
		if (nty != 0)
		{
			vy = -vy;
		}

		//TODO: Collision logic with dynamic object (bots)
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Eyeball::Render()
{
	int ani = EYEBALL_ANI_ALIVE;

	if (state == EYEBALL_STATE_DIE)
	{
		ani = EYEBALL_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y, nx);

	for (int i = 0; i < eyeballBulls.size(); i++)
	{
		if (eyeballBulls[i]->GetIsDestroyed())
		{
			delete eyeballBulls[i];
			eyeballBulls.erase(eyeballBulls.begin() + i);
		}
		else
			eyeballBulls[i]->Render();
	}

}

void Eyeball::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	nx = 1;
	vx = 0.03f;
}