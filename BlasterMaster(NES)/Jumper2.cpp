#include "Jumper2.h"

void Jumper2::Update(DWORD dt)
{
	CDynamicGameObject::Update(dt);
	if (vx > 0 && x > 290)
	{
		x = 290;
		SetState(JUMPER2_STATE_WALKING_LEFT);
	}
	if (vx < 0 && x < 0)
	{
		x = 0;
		SetState(JUMPER2_STATE_WALKING_RIGHT);
	}
}

void Jumper2::Render()
{
	int ani = JUMPER2_ANI_WALK_LEFT;
	/*if (vx == 0)
	{
		if (nx > 0) ani = INSECT_ANI_IDLE_RIGHT;
		else ani = INSECT_ANI_IDLE_LEFT;
	}
	else if (vx > 0)
		ani = INSECT_ANI_WALKING_RIGHT;
	else ani = INSECT_ANI_WALKING_LEFT;*/

	animation_set->at(ani)->Render(x, y, nx);
}

void Jumper2::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case JUMPER2_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case JUMPER2_STATE_WALKING_RIGHT:
		vx = JUMPER2_WALKING_SPEED;
		nx = 1;
		break;
	case JUMPER2_STATE_WALKING_LEFT:
		vx = -JUMPER2_WALKING_SPEED;
		nx = -1;
		break;
	default:
		break;
	}
}

