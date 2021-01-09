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
void Jumper2::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return;
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
						SetState(JUMPER2_STATE_WALKING_RIGHT);

					}
					else
					{
						SetState(JUMPER2_STATE_WALKING_LEFT);
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

