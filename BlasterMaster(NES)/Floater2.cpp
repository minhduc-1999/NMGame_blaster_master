#include "Floater2.h"

Floater2::Floater2(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(FLOATER2_WIDTH, FLOATER2_HEIGHT);
	vy = FLOATER2_FLYING_SPEED_Y;

	startTime = 0;
}

int Floater2::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
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
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		/*for (int i = 0; i < floaterBulls.size(); i++)
			floaterBulls[i]->Update(dt, coObjects);*/
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
					if (GetNX() == 1)
					{
						SetState(FLOATER2_STATE_FLYING_LEFT);

					}
					else
					{
						SetState(FLOATER2_STATE_FLYING_RIGHT);
					}
				}
				if (e->ny != 0)
				{
					vy = -vy;
				}
				break;
			case 20: //enemy bullet
				if (e->obj->GetTeam() == 0)
				{
					SetState(FLOATER2_STATE_DIE);
				}
				break;
			default:
				break;
			};

		}
		
		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	isUpdated = true;
	isRendered = false;
	return 0;
}

void Floater2::Render()
{
	if (isRendered)
		return;
	int ani = FLOATER2_ANI_FLYING;

	if (GetState() == FLOATER2_STATE_DIE)
	{
		ani = FLOATER2_ANI_DIE;
		if (!animation_set->at(FLOATER2_ANI_DIE)->IsCompleted())
		{
			animation_set->at(FLOATER2_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(FLOATER2_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}

	switch (state)
	{
	case FLOATER2_STATE_FLYING_RIGHT: case FLOATER2_STATE_FLYING_LEFT:
		ani = FLOATER2_ANI_FLYING;
		break;
	case FLOATER2_STATE_DIE:
		break;
	}

	animation_set->at(ani)->Render(x, y, nx);
	isRendered = true;
	isUpdated = false;
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
		SetSize(0, 0);
		vx = 0;
		vy = 0;
		break;
	}
}

vector<LPDYNAMICOBJECT> Floater2::Fire(float xMain, float yMain)
{
	vector<LPDYNAMICOBJECT> floaterBulls;

	Bullet* bullet = new Bullet(x, y, FLOATER_BULLET, 1);
	float a = xMain - x;
	float b = yMain - y;
	bullet->SetSpeed(a / sqrt(pow(a, 2) + pow(b, 2)) / 5, b / sqrt(pow(a, 2) + pow(b, 2)) / 5);
	floaterBulls.push_back(bullet);

	return floaterBulls;
}

int Floater2::FindMain(float xF, float yF, float xMain, float yMain)
{
	if (xF >= xMain)
	{
		if (yF >= yMain)
			return 1;
		return 2;
	}
	else
	{
		if (yF >= yMain)
			return 3;
		return 4;
	}
}