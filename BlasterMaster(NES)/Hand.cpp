#include "Hand.h"

Hand::Hand(float x, float y, int _index, bool _isLeftHand) :CDynamicGameObject(x, y)
{
	isLeftHand = _isLeftHand;
	SetTeam(1);
	if (isLeftHand)
	{
		SetPosition(x - 20, y);
		handPosition = D3DXVECTOR2(x - 20, y);
		currentPointIndex = 0;
		nextPointIndex = 1;
		lastTime = GetTickCount();
	}
	else
	{
		SetPosition(x + 20, y);
		handPosition = D3DXVECTOR2(x + 20, y);
		currentPointIndex = 3;
		nextPointIndex = 2;
		lastTime = GetTickCount() /*+ 500*/;
	}
	currentPosition = handPosition;
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
	
	vy = 0.08f;
}

int Hand::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR2 _bossPosition, int _nextPointIndex)
{
	if (state == HAND_STATE_DIE)
	{
		isUpdated = true;
		isRendered = false;
		return 0;
	}
	if (isUpdated)
		return -1;
	

	x += (_bossPosition.x - bossPosition.x);
	y += (_bossPosition.y - bossPosition.y);
	bossPosition = _bossPosition;
	handPosition = GetPosition();

	if (isLeftHand)
	{
		leftSide = _bossPosition.x - (20 + 10 * index);
		rightSide = _bossPosition.x - 20;
	}
	else
	{
		leftSide = _bossPosition.x + 20;
		rightSide = _bossPosition.x + (20 + 10 * index);
	}
	topSide = _bossPosition.y - 10 * (index);
	bottomSide = _bossPosition.y + 10 * (index);

	switch (nextPointIndex)
	{
	case 0:
		nextPosition.x = rightSide;
		nextPosition.y = _bossPosition.y;
		break;
	case 1:
		nextPosition.x = rightSide;
		nextPosition.y = bottomSide;
		break;
	case 2:
		nextPosition.x = leftSide;
		nextPosition.y = bottomSide;
		break;
	case 3:
		nextPosition.x = leftSide;
		nextPosition.y = _bossPosition.y;
		break;
	case 4:
		nextPosition.x = leftSide;
		nextPosition.y = topSide;
		break;
	case 5:
		nextPosition.x = rightSide;
		nextPosition.y = topSide;
		break;
	default:
		break;
	}
	switch (currentPointIndex)
	{
	case 0:
		currentPosition.x = rightSide;
		currentPosition.y = _bossPosition.y;
		break;
	case 1:
		currentPosition.x = rightSide;
		currentPosition.y = bottomSide;
		break;
	case 2:
		currentPosition.x = leftSide;
		currentPosition.y = bottomSide;
		break;
	case 3:
		currentPosition.x = leftSide;
		currentPosition.y = _bossPosition.y;
		break;
	case 4:
		currentPosition.x = leftSide;
		currentPosition.y = topSide;
		break;
	case 5:
		currentPosition.x = rightSide;
		currentPosition.y = topSide;
		break;
	default:
		break;
	}
	if (CalcD(handPosition, nextPosition) <= 5)
	{
		vx = 0;
		vy = 0;
		handPosition = nextPosition;
		SetPosition(nextPosition.x, nextPosition.y);
		if (GetTickCount() - lastTime >= 3000)
		{
			lastTime = GetTickCount();
			currentPointIndex = nextPointIndex;
			if (index == 5)
			{
				if (isLeftHand)
				{
					while (nextPointIndex == currentPointIndex)
					{
						nextPointIndex = rand() % 5 + 1;
					}
				}
				else
				{
					while (nextPointIndex == currentPointIndex || nextPointIndex == 3)
					{
						nextPointIndex = rand() % 6;
					}
				}
			}
			else
			{
				nextPointIndex = _nextPointIndex;
			}
			switch (nextPointIndex)
			{
			case 0:
				nextPosition.x = rightSide;
				nextPosition.y = _bossPosition.y;
				break;
			case 1:
				nextPosition.x = rightSide;
				nextPosition.y = bottomSide;
				break;
			case 2:
				nextPosition.x = leftSide;
				nextPosition.y = bottomSide;
				break;
			case 3:
				nextPosition.x = leftSide;
				nextPosition.y = _bossPosition.y;
				break;
			case 4:
				nextPosition.x = leftSide;
				nextPosition.y = topSide;
				break;
			case 5:
				nextPosition.x = rightSide;
				nextPosition.y = topSide;
				break;
			default:
				break;
			}
			switch (currentPointIndex)
			{
			case 0:
				currentPosition.x = rightSide;
				currentPosition.y = _bossPosition.y;
				break;
			case 1:
				currentPosition.x = rightSide;
				currentPosition.y = bottomSide;
				break;
			case 2:
				currentPosition.x = leftSide;
				currentPosition.y = bottomSide;
				break;
			case 3:
				currentPosition.x = leftSide;
				currentPosition.y = _bossPosition.y;
				break;
			case 4:
				currentPosition.x = leftSide;
				currentPosition.y = topSide;
				break;
			case 5:
				currentPosition.x = rightSide;
				currentPosition.y = topSide;
				break;
			default:
				break;
			}
			float a = nextPosition.x - currentPosition.x;
			float b = nextPosition.y - currentPosition.y;
			if (index != 5)
			{
				SetSpeed((a / sqrt(pow(a, 2) + pow(b, 2)) / 5) / (6 - index), (b / sqrt(pow(a, 2) + pow(b, 2)) / 5) / (6 - index));
			}
			else
			{
				SetSpeed((a / sqrt(pow(a, 2) + pow(b, 2)) / 5)/(1.5f), (b / sqrt(pow(a, 2) + pow(b, 2)) / 5)/(1.5f));
			}
		}
		
	}
	
	CDynamicGameObject::Update(dt);
	x += dx;
	y += dy;
	

	//handPosition = GetPosition();

	isUpdated = true;
	isRendered = false;
	return 0;
}

float Hand::CalcD(D3DXVECTOR2 a, D3DXVECTOR2 b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

void Hand::Render()
{
	if (isRendered)
		return;
	int ani = HAND_ANI;

	int dir = (isLeftHand)?1:-1;

	if (state == HAND_STATE_DIE)
	{
		animation_set->at(ani)->Render(x, y, dir, 253);
		if (GetTickCount64() - detroyTime >= 5000)
		{
			isDestroyed = true;
		}
	}
	else
	{
		animation_set->at(ani)->Render(x, y, dir);
	}
	

	isRendered = true;
	isUpdated = false;
}

void Hand::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case HAND_STATE_FLYING:
		break;
	case HAND_STATE_DIE:
		vx = 0;
		vy = 0;
		detroyTime = GetTickCount64();
		break;
	}
}