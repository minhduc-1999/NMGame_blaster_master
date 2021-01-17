#include "Jumper2.h"
Jumper2::Jumper2(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(18, 26);
	vx = JUMPER2_WALKING_SPEED;
	vy = JUMPER2_GRAVITY;
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
	vy = JUMPER2_GRAVITY;
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
		float min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby);
		if (nbx != 0)
		{
			x += min_tbx * dx + nbx * 0.4f;
			if (GetNX() == 1)
			{
				SetState(JUMPER2_STATE_WALKING_LEFT);
			}
			else
			{
				SetState(JUMPER2_STATE_WALKING_RIGHT);
			}
		}
		else
			x += dx;

		if (nby != 0)
		{
			y += min_tby * dy + nby * 0.4f;
			for (UINT i = 0; i < coEvents.size(); i++)
			{
				if (coEvents[i]->ny < 0)
				{
					if (IsJumping())
						vy = -JUMPER2_GRAVITY*5;
				}
			}
		}
		else
			y += dy;



		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int coObjType = e->obj->GetType();
			switch (coObjType)
			{
			case 17:	//brick and gate
				if (e->nx != 0)
				{
					if (GetNX() == 1)
					{
						SetState(JUMPER2_STATE_WALKING_LEFT);
						break;
					}
					else
					{
						SetState(JUMPER2_STATE_WALKING_RIGHT);
						break;
					}
				}
				if (e->ny != 0)
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
				break;
				//	if (e->ny != 0)
				//	{
				//		for (UINT i = 0; i < coEvents.size(); i++)
				//		{
				//			if (coEvents[i]->ny < 0)
				//			{
				//				if (IsJumping()) {
				//						vy = -JUMPER2_GRAVITY;
				//				}
				//			}
				//		}
				//	}
				//	break;
			case 20: //enemy bullet
				if (e->obj->GetTeam() == 0)
				{
					SetState(JUMPER2_STATE_DIE);
				}
				break;
			default:
				break;
			};

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

	if (GetState() == JUMPER2_STATE_DIE)
	{
		ani = JUMPER2_ANI_DIE;
		if (!animation_set->at(JUMPER2_ANI_DIE)->IsCompleted())
		{
			animation_set->at(JUMPER2_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(JUMPER2_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}

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
	case JUMPER2_STATE_DIE:
		SetSize(0, 0);
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}
}

