#include "Mine.h"
#include <cmath>

void Mine::Update(DWORD dt)
{
	GameObject::Update(dt);
}

void Mine::Render()
{
	int ani = MINE_ANI_ONGROUND;


	if (state == MINE_STATE_DIE)
	{
		ani = MINE_ANI_DIE;
	}

	animations[ani]->Render(x, y);
}
void Mine::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MINE_STATE_ONGROUND:
		break;
	case MINE_STATE_DIE:
		break;
	}
}
