#include "Jumper2.h"
Jumper2::Jumper2(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(18, 26);
	vy = JUMPER2_WALKING_SPEED;
}
bool Jumper2::IsJumping()
{
	if (vy < 0) return false;
	return true;
}
int Jumper2::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	CDynamicGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	vy += JUMPER2_GRAVITY;
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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//// if e->obj is Gate 
			//if (dynamic_cast<CDynamicGameObject*>(e->obj))
			//{
			//	x += (1 - min_tx) * dx - ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			//	y += (1 - min_ty) * dy - nty * 0.4f;
			//	/*CDynamicGameObject* obj = dynamic_cast<CDynamicGameObject*>(e->obj);
			//	if (this->team == obj->GetTeam())
			//	{*/					
			//		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
			//		return;
			//	//}
			//}
		}

		if (ntx != 0)
		{
			if (GetNX() == 1)
			{
				SetState(JUMPER2_STATE_WALKING_RIGHT);

			}
			else
			{
				SetState(JUMPER2_STATE_WALKING_LEFT);
			}
		}
		if (nty != 0)
		{
			for (UINT i = 0; i < coEvents.size(); i++)
			{
				if (coEvents[i]->ny < 0)
				{
					if (IsJumping()) {
						jump--;
						if (jump < 0)
						{
							vy = -vy;
							vx = JUMPER2_WALKING_SPEED;
						}
					}
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		//TODO: Collision logic with dynamic object (bots)
	}
	isUpdated = true;
	isRendered = false;
	return 0;
}

void Jumper2::Render()
{
	if (isRendered)
		return;
	int ani = JUMPER2_ANI_WALK;
	switch (state)
	{
	case JUMPER2_STATE_WALKING_LEFT:
		ani = JUMPER2_ANI_WALK;
		break;
	case JUMPER2_STATE_WALKING_RIGHT:
		ani = JUMPER2_ANI_WALK;
		break;
	}

	animation_set->at(ani)->Render(x, y, nx);
	isRendered = true;
	isUpdated = false;
}

void Jumper2::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case JUMPER2_STATE_WALKING_RIGHT:
		vx = JUMPER2_WALKING_SPEED;
		vy = JUMPER2_GRAVITY;
		nx = 1;
		break;
	case JUMPER2_STATE_WALKING_LEFT:
		vx = -JUMPER2_WALKING_SPEED;
		vy = JUMPER2_GRAVITY;
		nx = -1;
		break;
	case JUMPER2_STATE_JUMPING_RIGHT:
		break;
	default:
		break;
	}
}

