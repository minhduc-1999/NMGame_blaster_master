#include "Hand.h"

Hand::Hand(float x, float y, int _index, bool _isLeftHand) :CDynamicGameObject(x, y)
{
	isLeftHand = _isLeftHand;
	if (isLeftHand)
	{
		SetPosition(x - 20, y);
		handPosition = D3DXVECTOR2(x - 20, y);
	}
	else
	{
		SetPosition(x + 20, y);
		handPosition = D3DXVECTOR2(x + 20, y);
	}
	index = _index;
	bossPosition = D3DXVECTOR2(x, y);
	if (index != 5)
	{
		SetSize(ARM_WIDTH, ARM_HEIGHT);
	}
	else
	{
		SetSize(HAND_WIDTH, HAND_HEIGHT);
	}
	nx = (isLeftHand)?1:-1;
	ny = 1;
}

int Hand::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR2 _bossPosition)
{
	if (isUpdated)
		return -1;
	CDynamicGameObject::Update(dt);

	x += (_bossPosition.x - bossPosition.x);
	y += (_bossPosition.y - bossPosition.y);
	bossPosition = _bossPosition;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	if (isLeftHand)
	{
		leftSide = _bossPosition.x - (20 + 15*index);
		rightSide = _bossPosition.x - 20;
	}
	else
	{
		leftSide = _bossPosition.x + 20;
		rightSide = _bossPosition.x + (20 + 15 * index);
	}

	topSide = _bossPosition.y - 18 *(index);
	bottomSide = _bossPosition.y + 18* (index);

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	//if (coEvents.size() == 0)
	//{
	//	x += dx;
	//	y += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, ntx, nty;

	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty);

	//	// block 
	//	x += min_tx * dx + ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
	//	y += min_ty * dy + nty * 0.4f;

	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];
	//		// if e->obj is Gate 
	//		if (dynamic_cast<CDynamicGameObject*>(e->obj))
	//		{
	//			CDynamicGameObject* obj = dynamic_cast<CDynamicGameObject*>(e->obj);
	//			if (this->team == obj->GetTeam())
	//			{
	//				x += (1 - min_tx) * dx - ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
	//				y += (1 - min_ty) * dy - nty * 0.4f;
	//				for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//				return;
	//			}
	//		}
	//	}
	//	//TODO: Collision logic with dynamic object (bots)
	//}

	x += dx;
	y += dy;
	if (index != 5)
	{
		if (x < leftSide || x > rightSide)
		{
			x -= dx;
			vx = 0;
			vy = 0;
		}
		else if (y < topSide || y > bottomSide)
		{
			y -= dy;
			vx = 0;
			vy = 0;
		}
	}
	else
	{
		if (x < leftSide || x > rightSide)
		{
			x -= dx;
			if (lastTime == 0)
			{
				lastTime = GetTickCount();
			}
			else
			{
				vx = 0;
				vy = 0;
			}
			DWORD now = GetTickCount();
			if (lastTime != 0 && now - lastTime >= 1500)
			{
				SetState((ny == 1) ? HAND_STATE_FLYING_UP : HAND_STATE_FLYING_DOWN);
				lastTime = 0;
			}
			
		}
		else if (y < topSide || y > bottomSide)
		{
			y -= dy;
			if (lastTime == 0)
			{
				lastTime = GetTickCount();
			}
			else
			{
				vx = 0;
				vy = 0;
			}
			DWORD now = GetTickCount();
			if (lastTime != 0 && now - lastTime >= 1500)
			{
				SetState((nx == 1) ? HAND_STATE_FLYING_LEFT : HAND_STATE_FLYING_RIGHT);
				lastTime = 0;
			}
			
		}
	}
	

	handPosition = GetPosition();

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	isUpdated = true;
	isRendered = false;
	return 0;
}

void Hand::Render()
{
	if (isRendered)
		return;
	int ani = HAND_ANI;

	int dir = (isLeftHand)?1:-1;

	animation_set->at(ani)->Render(x, y, dir);

	isRendered = true;
	isUpdated = false;
}

void Hand::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case HAND_STATE_FLYING_BIAS:
		if (vx == 0)
		{
			ny = -ny;
			if (ny == -1)
			{
				SetState(HAND_STATE_FLYING_UP);
			}
			else
			{
				SetState(HAND_STATE_FLYING_DOWN);
			}
		}
		else
		{
			nx = -nx;
			if (nx == -1)
			{
				SetState(HAND_STATE_FLYING_RIGHT);
			}
			else
			{
				SetState(HAND_STATE_FLYING_LEFT);
			}
		}
		break;
	case HAND_STATE_FLYING_LEFT:
		nx = -1;
		vx = index * HAND_FLYING_SPEED_X * nx;
		vy = 0;
		break;
	case HAND_STATE_FLYING_UP:
		ny = -1;
		vy = index * HAND_FLYING_SPEED_Y * ny;
		vx = 0;
		break;
	case HAND_STATE_FLYING_RIGHT:
		nx = 1;
		vx = index * HAND_FLYING_SPEED_X * nx;
		vy = 0;
		break;
	case HAND_STATE_FLYING_DOWN:
		ny = 1;
		vy = index * HAND_FLYING_SPEED_Y * ny;
		vx = 0;
		break;
	}
}