#include "Orb.h"

void Orb::Update(DWORD dt)
{
	CDynamicGameObject::Update(dt);
	if (vx > 0 && x > 290)
	{
		x = 290;
		SetState(ORB_STATE_TURN_LEFT);
	}
	else if (vx < 0 && x < 0)
	{
		x = 0;
		SetState(ORB_STATE_TURN_RIGHT);
	}
	else if (vx == 0 && vy == 0)
	{
		if (animation_set->at(ORB_ANI_TURN_RIGHT)->IsCompleted())
		{
			if (nx == 1)
				SetState(ORB_STATE_RUNNING_LEFT);
			else
				SetState(ORB_STATE_RUNNING_RIGHT);
			animation_set->at(ORB_ANI_TURN_RIGHT)->ResetAnim();
		}
	}
}

void Orb::Render()
{
	int ani = ORB_ANI_RUN_LEFT;
	switch (state)
	{
	case ORB_STATE_TURN_RIGHT:
		ani = ORB_ANI_TURN_RIGHT;
		break;
	case ORB_STATE_TURN_LEFT:
		ani = ORB_ANI_TURN_RIGHT;
		break;
	case ORB_STATE_RUNNING_LEFT:
		ani = ORB_ANI_RUN_LEFT;
		break;
	case ORB_STATE_RUNNING_RIGHT:
		ani = ORB_ANI_RUN_LEFT;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(x, y, nx);
}

void Orb::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case ORB_STATE_TURN_RIGHT:
		vx = 0;
		vy = 0;
		nx = -1;
		break;
	case ORB_STATE_TURN_LEFT:
		vx = 0;
		vy = 0;
		nx = 1;
		break;
	case ORB_STATE_RUNNING_LEFT:
		vx = -ORB_RUNNING_SPEED;
		nx = -1;
		break;
	case ORB_STATE_RUNNING_RIGHT:
		vx = ORB_RUNNING_SPEED;
		nx = 1;
		break;
	default:
		break;
	}
}

