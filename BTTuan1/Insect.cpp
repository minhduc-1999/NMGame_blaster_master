#include "Insect.h"

void Insect::Update(DWORD dt)
{
	/*switch (state)
	{
	case INSECT_STATE_FLYING_RIGHT:
		vx = INSECT_FLYING_SPEED;
		nx = 1;
		break;
	case INSECT_STATE_FLYING_LEFT:
		vx = -INSECT_FLYING_SPEED;
		nx = -1;
		break;
	case INSECT_STATE_IDLE:
		vx = 0;
		break;
	}*/

	// simple fall down

	//// simple screen edge collision!!!
	CGameObject::Update(dt);
	if (vx > 0 && x > 290)
	{
		x = 290;
		SetState(INSECT_STATE_FLYING_LEFT);
	}
	if (vx < 0 && x < 0)
	{
		x = 0;
		SetState(INSECT_STATE_FLYING_RIGHT);
	}

}

void Insect::Render()
{
	int ani = INSECT_ANI_FLY_LEFT;
	/*if (vx == 0)
	{
		if (nx > 0) ani = INSECT_ANI_IDLE_RIGHT;
		else ani = INSECT_ANI_IDLE_LEFT;
	}
	else if (vx > 0)
		ani = INSECT_ANI_FLYING_RIGHT;
	else ani = INSECT_ANI_FLYING_LEFT;*/
	
	animations[ani]->Render(x, y, nx);
}

void Insect::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case INSECT_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case INSECT_STATE_FLYING_RIGHT:
		vx = INSECT_FLYING_SPEED;
		nx = 1;
		break;
	case INSECT_STATE_FLYING_LEFT:
		vx = -INSECT_FLYING_SPEED;
		nx = -1;
		break;
	default:
		break;
	}
}

