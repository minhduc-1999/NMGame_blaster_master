#include "JasonBullet.h"

JasonBullet::JasonBullet(float x, float y, int team, int nx, int ny) : BaseBullet(x, y, team, nx, ny)
{
	this->startFiringTime = GetTickCount64();
	this->damage = JASON_BULLET_DAMAGE;
	this->SetAnimationSet(JASON_BULLET_ANISET);
	if (nx != 0)
	{
		this->SetSize(10, 8);
		if (nx == 1)
		{
			SetState(JASON_BULLET_STATE_FLY_RIGHT);
			SetPosition(x + 8, y);
		}
		else
		{
			SetState(JASON_BULLET_STATE_FLY_LEFT);
			SetPosition(x - 8, y);
		}
			
	}
	else
	{
		this->SetSize(8, 10);
		if (ny == 1)
		{
			SetState(JASON_BULLET_STATE_FLY_BOTTOM);
			SetPosition(x, y + 12);

		}
		else
		{
			SetState(JASON_BULLET_STATE_FLY_TOP);
			SetPosition(x, y - 12);
		}
	}
	isUpdated = false;
	isRendered = false;
}

int JasonBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return -1;
	if (state == JASON_BULLET_STATE_DETROY)
		return 0;
	if (GetTickCount64() - startFiringTime >= JASON_BULLET_ALIVE_TIME)
	{
		SetState(JASON_BULLET_STATE_DETROY);
		isDestroyed = true;
		return 0;
	}
	CDynamicGameObject::Update(dt);
	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		int objTeam = temp->GetTeam();
		if (objTeam != this->team && temp->GetType() != 16)
		{
			if (objTeam == 1)
			{
				LPDYNAMICOBJECT dyn = dynamic_cast<LPDYNAMICOBJECT>(temp);
				dyn->SetIsDestroyed();
			}
			SetState(JASON_BULLET_STATE_DETROY);
			//isDestroyed = true;
			isUpdated = true;
			isRendered = false;
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
			if (objTeam != this->team && e->obj->GetType() != 16)
			{
				if (objTeam == 1)
				{
					LPDYNAMICOBJECT dyn = dynamic_cast<LPDYNAMICOBJECT>(e->obj);
					dyn->SetIsDestroyed();
				}
				SetState(JASON_BULLET_STATE_DETROY);
				//isDestroyed = true;
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

void JasonBullet::Render()
{
	if (isRendered)
		return;
	int origin = -1;
	int ani = JASON_BULLET_ANI_FLY_HOR;
	switch (state)
	{
	case JASON_BULLET_STATE_DETROY:
		ani = JASON_BULLET_ANI_DETROY;
		break;
	case JASON_BULLET_STATE_FLY_LEFT:
		origin = -1;
		break;
	case JASON_BULLET_STATE_FLY_RIGHT:
		origin = 1;
		break;
	case JASON_BULLET_STATE_FLY_BOTTOM:
		ani = JASON_BULLET_ANI_FLY_VER;
		origin = 1;
		break;
	case JASON_BULLET_STATE_FLY_TOP:
		ani = JASON_BULLET_ANI_FLY_VER;
		origin = -1;
		break;
	}

	if (state == JASON_BULLET_STATE_FLY_BOTTOM || state == JASON_BULLET_STATE_FLY_TOP)
	{
		animation_set->at(ani)->RenderFlipY(x, y, origin, 255);
	}
	else
	{
		animation_set->at(ani)->Render(x, y, origin);
	}
	if (state == JASON_BULLET_STATE_DETROY)
		this->isDestroyed = true;
	isUpdated = false;
	isRendered = true;
}

void JasonBullet::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case JASON_BULLET_STATE_DETROY:
		vx = 0;
		vy = 0;
		break;
	case JASON_BULLET_STATE_FLY_LEFT:
		vx = -JASON_BULLET_SPEED;
		vy = 0;
		nx = -1;
		ny = 0;
		break;
	case JASON_BULLET_STATE_FLY_RIGHT:
		vx = JASON_BULLET_SPEED;
		vy = 0;
		nx = 1;
		ny = 0;
		break;
	case JASON_BULLET_STATE_FLY_TOP:
		vx = 0;
		vy = -JASON_BULLET_SPEED;
		nx = 0;
		ny = -1;
		break;
	case JASON_BULLET_STATE_FLY_BOTTOM:
		vx = 0;
		vy = JASON_BULLET_SPEED;
		nx = 0;
		ny = 1;
		break;
	}
}