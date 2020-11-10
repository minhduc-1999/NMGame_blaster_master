#include "Player.h"

int lastHeight = 0;
int currentWalkingColumn = 0;


Player::Player(float x, float y) :CDynamicGameObject(x, y)
{
	height = PLAYER_HEIGHT_HIGH;
};

void Player::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vy += PLAYER_GRAVITY;

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
		x += min_tx * dx + ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + nty * 0.4f;

		if (ntx != 0) vx = 0;
		if (nty != 0) vy = 0;

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Player::Render()
{
	int ani = SOPHIA_ANI_RUN_HIGH;

	switch (state)
	{
	case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT:
		ani = SOPHIA_ANI_RUN_HIGH;
		animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
		return;
		break;
	case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
		if (height == PLAYER_HEIGHT_HIGH)
		{
			lastHeight = 0;
			ani = SOPHIA_ANI_RUN_HIGH;
			height++;
		}
		else if (height == PLAYER_HEIGHT_LOW)
		{
			lastHeight = 2;
			ani = SOPHIA_ANI_RUN_LOW;
			height--;
		}
		else
		{
			if (lastHeight == 0)
			{
				ani = SOPHIA_ANI_RUN_HIGH;
				height++;
			}
			else
			{
				ani = SOPHIA_ANI_RUN_LOW;
				height--;
			}
		}

		if (currentWalkingColumn == 3)
		{
			currentWalkingColumn = 0;
		}
		else
		{
			currentWalkingColumn++;
		}

		animations[ani]->RenderStartByFrame(currentWalkingColumn, x, y, nx);
		return;
		break;
	case SOPHIA_STATE_JUMP_RIGHT:case SOPHIA_STATE_JUMP_LEFT:
		ani = SOPHIA_ANI_JUMP_UP;
		break;
	case SOPHIA_STATE_TURN_RUN:
		ani = SOPHIA_ANI_TURN_RUN;
		animations[ani]->Render(x, y, nx);
		if (animations[ani]->IsCompleted())
		{
			if (nx == -1)
			{
				SetState(SOPHIA_STATE_RUN_RIGHT);
			}
			else
			{
				SetState(SOPHIA_STATE_RUN_LEFT);
			}
		}
		return;
		break;
	}

	animations[ani]->Render(x, y, nx);
}

void Player::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case SOPHIA_STATE_IDLE_RIGHT:
		vx = 0;
		nx = 1;
		break;
	case SOPHIA_STATE_IDLE_LEFT:
		vx = 0;
		nx = -1;
		break;
	case SOPHIA_STATE_RUN_RIGHT:
		vx = PLAYER_RUN_SPEED;
		nx = 1;
		break;
	case SOPHIA_STATE_RUN_LEFT:
		vx = -PLAYER_RUN_SPEED;
		nx = -1;
		break;
	case SOPHIA_STATE_JUMP_RIGHT:
		vy = -PLAYER_JUMP_SPEED_Y;
		nx = 1;
		break;
	case SOPHIA_STATE_JUMP_LEFT:
		vy = -PLAYER_JUMP_SPEED_Y;
		nx = -1;
		break;
	case SOPHIA_STATE_TURN_RUN:
		break;
	}
}