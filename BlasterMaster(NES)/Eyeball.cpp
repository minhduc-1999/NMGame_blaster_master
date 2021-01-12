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

int Eyeball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == EYEBALL_STATE_DIE)
	{
		return 0;
	}
	CDynamicGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	startTime += dt;

	if (startTime > 800)
	{
		isShooting = true;
		startTime = 0;
	}

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
					vx = -vx;
				}
				if (e->ny != 0)
				{
					vy = -vy;
				}
				break;
			case 20: //enemy bullet
				if (e->obj->GetTeam() == 0)
				{
					SetState(EYEBALL_STATE_DIE);
				}
				break;
			default:
				break;
			};

		}
		//TODO: Collision logic with dynamic object (bots)
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	return 0;
}

void Eyeball::Render()
{
	int ani = EYEBALL_ANI_ALIVE;

	if (state == EYEBALL_STATE_DIE)
	{
		ani = EYEBALL_ANI_DIE;
		if (!animation_set->at(EYEBALL_ANI_DIE)->IsCompleted())
		{
			animation_set->at(EYEBALL_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(EYEBALL_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}

	animation_set->at(ani)->Render(x, y, nx);

}

void Eyeball::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	if (state == EYEBALL_STATE_DIE)
	{
		SetSize(0, 0);
		vx = 0;
		vy = 0;
	}
	nx = 1;
	vx = 0.03f;
}

vector<LPDYNAMICOBJECT> Eyeball::Fire(float xMain, float yMain)
{
	vector<LPDYNAMICOBJECT> eyeballBulls;
	Bullet* bullet = new Bullet(x + 7, y, EYEBALL_BULLET, 1);
	float a = xMain - x;
	float b = yMain - y;
	bullet->SetSpeed(a / sqrt(pow(a, 2) + pow(b, 2)) / 5, b / sqrt(pow(a, 2) + pow(b, 2)) / 5);
	eyeballBulls.push_back(bullet);
	
	return eyeballBulls;
}