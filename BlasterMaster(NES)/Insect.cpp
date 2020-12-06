#include "Insect.h"
Insect::Insect(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(18,	18);
	vy = INSECT_FLYING_SPEED;
}
void Insect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return;
	CDynamicGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	jump--;
	if (jump < 0)
	{
		vy = -vy;
		vx = INSECT_FLYING_SPEED * 5;
		jump = INSECT_JUMPING_TIME;
		vx = 0;
		vy = INSECT_FLYING_SPEED;
	}
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
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			// if e->obj is Gate 
			if (dynamic_cast<CDynamicGameObject*>(e->obj))
			{
				CDynamicGameObject* obj = dynamic_cast<CDynamicGameObject*>(e->obj);
				if (this->team == obj->GetTeam())
				{
					x += (1 - min_tx) * dx - ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
					y += (1 - min_ty) * dy - nty * 0.4f;
					for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
					return;
				}
			}
		}
		if (ntx != 0)
		{
			if (GetNX() == 1)
			{
				SetState(INSECT_STATE_FLYDOWN_RIGHT);

			}
			else
			{
				SetState(INSECT_STATE_FLYDOWN_LEFT);
			}
		}

		if (nty != 0)
		{
			vy = -INSECT_FLYING_SPEED*10;
			vx = INSECT_FLYING_SPEED*5;

		}

		//TODO: Collision logic with dynamic object (bots)
	}
	isUpdated = true;
	isRendered = false;
}

void Insect::Render()
{
	if (isRendered)
		return;
	int ani = INSECT_ANI_FLYING;
	switch (state)
	{
	case INSECT_STATE_FLYDOWN_LEFT:
		ani = INSECT_ANI_FLYING;
		break;
	case INSECT_STATE_FLYDOWN_RIGHT:
		ani = INSECT_ANI_FLYING;
		break;
	}
	
	animation_set->at(ani)->Render(x, y, nx);
	isRendered = true;
	isUpdated = false;
}

void Insect::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case INSECT_STATE_FLYDOWN_RIGHT:
		vx = 0;
		vy = INSECT_FLYING_SPEED;
		nx = 1;
		break;
	case INSECT_STATE_FLYDOWN_LEFT:
		vx = 0;
		vy = INSECT_FLYING_SPEED;
		nx = -1;
		break;
	default:
		break;
	}
}

