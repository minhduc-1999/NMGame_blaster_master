#include "Skull.h"
#include <cmath>

void Skull::Update(DWORD dt)
{
	CDynamicGameObject::Update(dt);
	/*if (vx > 0 && x > 290)
	{
		x = 290;
		SetState(SKULL_STATE_FLYING_LEFT);
	}
	else if (vx < 0 && x < 0)
	{
		x = 0;
		SetState(SKULL_STATE_FLYING_RIGHT);
	}
	else if (vx == 0 && vy == 0)
	{
		if (animations[SKULL_ANI_TURN_RIGHT]->IsCompleted())
		{
			if (nx == 1)
				SetState(ORB_STATE_RUNNING_LEFT);
			else
				SetState(ORB_STATE_RUNNING_RIGHT);
			animations[ORB_ANI_TURN_RIGHT]->ResetAnim();
		}
	}*/
}

void Skull::Render()
{
	int ani = SKULL_ANI_FLYING_LEFT;
	switch (state)
	{
	case SKULL_STATE_FLYING_LEFT:
		ani = SKULL_ANI_FLYING_LEFT;
		break;
	case SKULL_STATE_FLYING_RIGHT:
		ani = SKULL_ANI_FLYING_LEFT;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(x, y,nx);
}
void Skull::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case SKULL_STATE_FLYING_LEFT:
		vx = -SKULL_FLYING_SPEED;
		nx = -1;
		break;
	case SKULL_STATE_FLYING_RIGHT:
		vx = SKULL_FLYING_SPEED;
		nx = 1;
		break;
	default:
		break;
	}
}
