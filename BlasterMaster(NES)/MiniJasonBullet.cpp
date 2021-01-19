#include "MiniJasonBullet.h"

MiniJasonBullet::MiniJasonBullet(float x, float y, int team, int nx, int ny) : BaseBullet(x, y, team, nx, ny)
{
	SetSize(6, 6);
	SetAnimationSet(MINIJASON_BULLET_ANISET);
	if (nx != 0)
	{
		if (nx == 1)
		{
			SetState(MINIJASON_BULLET_STATE_RIGHT);
			SetPosition(x + 4, y);
		}
		else
		{
			SetState(MINIJASON_BULLET_STATE_LEFT);
			SetPosition(x - 4, y);
		}
	}
	isUpdated = false;
	isRendered = false;
}

int MiniJasonBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (state == MINIJASON_BULLET_STATE_DESTROY)
		return 0;

	if (GetTickCount64() - startFiringTime >= 300)
	{
		SetState(MINIJASON_BULLET_STATE_DESTROY);
		isDestroyed = true;
		return 0;
	}

	CDynamicGameObject::Update(dt, coObjects);

	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		int objTeam = temp->GetTeam();
		if (objTeam != this->team && !(temp->GetType() == 16 || temp->GetType() == 19))
		{
			if (objTeam == 1)
			{
				if (temp->GetType() != 117)
				{
					LPDYNAMICOBJECT dyn = dynamic_cast<LPDYNAMICOBJECT>(temp);
					dyn->SetIsDestroyed();
				}
				SetState(MINIJASON_BULLET_STATE_DESTROY);
			}
			else if (objTeam == -1)
			{
				//if (type == 0)
				//{
					SetState(MINIJASON_BULLET_STATE_DESTROY);
				//}
			}
			return 0;
		}
	}
	for (UINT i = 0; i < curCoEvents.size(); i++) delete curCoEvents[i];

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
		x += min_tx * dx + ntx * 0.4f;
		y += min_ty * dy + nty * 0.4f;

		//TODO: Collision logic with dynamic object (bots)
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int objTeam = e->obj->GetTeam();
			if (objTeam != this->team && !(e->obj->GetType() == 16 || e->obj->GetType() == 19))
			{
				if (objTeam == 1)
				{
					if (e->obj->GetType() != 117)
					{
						LPDYNAMICOBJECT dyn = dynamic_cast<LPDYNAMICOBJECT>(e->obj);
						dyn->SetIsDestroyed();
					}
					SetState(MINIJASON_BULLET_STATE_DESTROY);
				}
				else if (objTeam == -1)
				{
					//if (type == 0)
					//{
						SetState(MINIJASON_BULLET_STATE_DESTROY);
					//}
				}
				break;
			}
			if (e->obj->GetType() != 15)
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
		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut("[JASON-Bullet]\tx: %f, y: %f\n", x, y);
	isUpdated = true;
	isRendered = false;

	return 0;
}

void MiniJasonBullet::Render()
{
	if (isRendered)
		return;
	int dir = 1;
	int ani = MINIJASON_BULLET_ANI_FIRING;
	switch (state)
	{
	case MINIJASON_BULLET_STATE_DESTROY:
		ani = MINIJASON_BULLET_ANI_DESTROY;
		break;
	case MINIJASON_BULLET_STATE_RIGHT:
		break;
	case MINIJASON_BULLET_STATE_LEFT:
		break;
	}
	animation_set->at(ani)->Render(x, y, dir);
	if (state == MINIJASON_BULLET_STATE_DESTROY)
		isDestroyed = true;

	isUpdated = false;
	isRendered = true;
}

void MiniJasonBullet::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case MINIJASON_BULLET_STATE_DESTROY:
		vx = 0;
		vy = 0;
		break;
	case MINIJASON_BULLET_STATE_RIGHT:
		vx = MINIJASON_BULLET_SPEED;
		vy = 0;
		break;
	case MINIJASON_BULLET_STATE_LEFT:
		vx = -MINIJASON_BULLET_SPEED;
		vy = 0;
		break;
	}
}
