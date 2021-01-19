#include "Jumper2.h"
Jumper2::Jumper2(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(18, 26);
	startTime = GetTickCount64();
	SetState(JUMPER2_STATE_WALKING_RIGHT);
	countJump = 0;
	isUpdated = false;
	isRendered = false;
}

int Jumper2::Update(float xMain, float yMain, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (isDestroyed)
		return 0;

	CDynamicGameObject::Update(dt);

	vy += JUMPER2_GRAVITY;
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	// No collision occured, proceed normally

	startTime += dt;
	if (nx == 1)
		SetState(JUMPER2_STATE_WALKING_RIGHT);
	else
		SetState(JUMPER2_STATE_WALKING_LEFT);
	if (startTime > 700 && canJump)
	{
			if (nx == 1)
				SetState(JUMPER2_STATE_JUMPING_RIGHT);
			else
				SetState(JUMPER2_STATE_JUMPING_LEFT);
			//startTime = 600;
			//countJump++;
			canJump = false;
			startTime = 0;
	}
	//else
	//{
	//	if (nx == 1)
	//		SetState(JUMPER2_STATE_WALKING_RIGHT);
	//	else
	//		SetState(JUMPER2_STATE_WALKING_LEFT);
	//}

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby);
		if (nbx == 0)
			x += dx;
		if (nby == 0)
			y += dy;
		if (nby == -1)
		{
			if (abs(xMain - x) <= 100 && abs(yMain - y) <= 50)
			{
				canJump = true;
				if (xMain - x < 0)
					SetState(JUMPER2_STATE_JUMPING_LEFT);
				else
					SetState(JUMPER2_STATE_JUMPING_RIGHT);
			}
			else
			{
				if (nx == 1)
					SetState(JUMPER2_STATE_WALKING_RIGHT);
				else
					SetState(JUMPER2_STATE_WALKING_LEFT);
			}
			y += min_tby * dy + nby * 0.4f;
		}
		else if (nbx != 0 && nby != 0)
		{
			x += min_tbx * dx + nbx * 0.4f;
			if (nbx == -1)
				SetState(JUMPER2_STATE_WALKING_LEFT);
			else
				SetState(JUMPER2_STATE_WALKING_RIGHT);
			vy += JUMPER2_GRAVITY;
		}
		else if (nbx != 0 && nby == 0)
		{
			x += min_tbx * dx + nbx * 0.4f;
			if (nbx == -1)
				SetState(JUMPER2_STATE_WALKING_LEFT);
			else
				SetState(JUMPER2_STATE_WALKING_RIGHT);
		}
		else if (nby != 0)
		{
			y += min_tby * dy + nby * 0.4f;
			if (nby == -1)
			{
				canJump = true;
				vy = -JUMPER2_JUMPING_SPEED;
				ny = -ny;
			}
			else
				vy += JUMPER2_GRAVITY;
		}
		/*else
			y += dy;*/

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int coObjType = e->obj->GetType();
			switch (coObjType)
			{
			case 17:
				if (e->nx != 0)
				{
					if (e->nx == 1)
						SetState(JUMPER2_STATE_WALKING_RIGHT);
					else
						SetState(JUMPER2_STATE_WALKING_LEFT);
					break;
				}
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
		//TODO: Collision logic with dynamic object (bots)
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

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

	animation_set->at(ani)->Render(x, y, nx);
	isRendered = true;
	isUpdated = false;
}

void Jumper2::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case JUMPER2_STATE_JUMPING_RIGHT:
		vx = JUMPER2_WALKING_SPEED*1,5;
		vy = -JUMPER2_JUMPING_SPEED;
		nx = 1;
		break;
	case JUMPER2_STATE_JUMPING_LEFT:
		vx = -JUMPER2_WALKING_SPEED*1,5;
		vy = -JUMPER2_JUMPING_SPEED;
		nx = -1;
		break;
	case JUMPER2_STATE_WALKING_LEFT:
		vx = -JUMPER2_WALKING_SPEED;
		vy += JUMPER2_GRAVITY;
		nx = -1;
		break;
	case JUMPER2_STATE_WALKING_RIGHT:
		vx = JUMPER2_WALKING_SPEED;
		vy += JUMPER2_GRAVITY;
		nx = 1;
		break;
	case JUMPER2_STATE_DIE:
		SetSize(0, 0);
		vx = 0;
		vy = 0;
		break;
	default:
		vx = JUMPER2_WALKING_SPEED;
		vy += JUMPER2_GRAVITY;
		nx = 1;
		break;
	}
}

