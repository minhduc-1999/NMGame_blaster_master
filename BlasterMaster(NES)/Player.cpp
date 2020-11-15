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

		if (ntx != 0)
		{
			
			if (GetNX() == 1)
			{
				SetState(SOPHIA_STATE_IDLE_RIGHT);

			}
			else
			{
				SetState(SOPHIA_STATE_IDLE_LEFT);
			}
		}

		if (nty != 0)
		{
			SetIsJumping(false);
			vy = 0;
		}

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Player::Render()
{
	int ani = -1;

	if (GetIsJumping())
	{
		if (GetIsUp())
		{
			animations[SOPHIA_ANI_DOWN]->ResetAnim();
			switch (state)
			{
			case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT:
				if (vy < 0)
				{
					ani = SOPHIA_ANI_UP_JUMP;
				}
				else
				{
					ani = SOPHIA_ANI_UP_RUN_LOW;
				}
				animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;
			case SOPHIA_STATE_JUMP_RIGHT:case SOPHIA_STATE_JUMP_LEFT:
				if (vy < 0)
				{
					ani = SOPHIA_ANI_UP_JUMP;
				}
				else
				{
					ani = SOPHIA_ANI_UP_RUN_LOW;
				}
				animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				ani = SOPHIA_ANI_UP_JUMP;
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
			}
		}
		else
		{
			animations[SOPHIA_ANI_UP]->ResetAnim();
			switch (state)
			{
			case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT:
				if (vy < 0)
				{
					ani = SOPHIA_ANI_JUMP_UP;
				}
				else
				{
					ani = SOPHIA_ANI_JUMP_DOWN;
				}
				animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;
			case SOPHIA_STATE_JUMP_RIGHT:case SOPHIA_STATE_JUMP_LEFT:
				if (vy < 0)
				{
					ani = SOPHIA_ANI_JUMP_UP;
				}
				else
				{
					ani = SOPHIA_ANI_JUMP_DOWN;
				}
				animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				if (vy < 0)
				{
					ani = SOPHIA_ANI_JUMP_UP;
				}
				else
				{
					ani = SOPHIA_ANI_JUMP_DOWN;
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
			case SOPHIA_STATE_TURN_RUN:
				ani = SOPHIA_ANI_TURN_JUMP;
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
		}
	}
	else
	{
		if (GetIsUp())
		{
			animations[SOPHIA_ANI_DOWN]->ResetAnim();
			switch (state)
			{
			case SOPHIA_STATE_IDLE_RIGHT: case SOPHIA_STATE_IDLE_LEFT:
				ani = SOPHIA_ANI_UP;
				if (animations[ani]->IsCompleted())
				{
					ani = SOPHIA_ANI_UP_RUN_HIGH;
					animations[ani]->RenderFrame(currentWalkingColumn,x, y, nx);
				}
				else
				{
					animations[ani]->Render(x, y, nx);
				}
				//ani = SOPHIA_ANI_UP_RUN_HIGH;
				//animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;
			case SOPHIA_STATE_JUMP_RIGHT: case SOPHIA_STATE_JUMP_LEFT:
				/*ani = SOPHIA_ANI_UP;
				if (animations[ani]->IsCompleted())
				{
					ani = SOPHIA_ANI_UP_RUN_HIGH;
					animations[ani]->RenderFrame(currentWalkingColumn,x, y, nx);
				}
				else
				{
					animations[ani]->Render(x, y, nx);
				}*/
				ani = SOPHIA_ANI_UP_RUN_HIGH;
				animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				if (height == PLAYER_HEIGHT_HIGH)
				{
					lastHeight = 0;
					ani = SOPHIA_ANI_UP_RUN_HIGH;
					height++;
				}
				else if (height == PLAYER_HEIGHT_LOW)
				{
					lastHeight = 2;
					ani = SOPHIA_ANI_UP_RUN_LOW;
					height--;
				}
				else
				{
					if (lastHeight == 0)
					{
						ani = SOPHIA_ANI_UP_RUN_HIGH;
						height++;
					}
					else
					{
						ani = SOPHIA_ANI_UP_RUN_LOW;
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
			/*case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT:
				ani = SOPHIA_ANI_UP_RUN_HIGH;
				animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;*/
			}
		}
		else
		{
			animations[SOPHIA_ANI_UP]->ResetAnim();
			switch (state)
			{
			case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT:
				ani = SOPHIA_ANI_DOWN;
				if (animations[ani]->IsCompleted())
				{
					ani = SOPHIA_ANI_RUN_HIGH;
					animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
				}
				else
				{
					animations[ani]->Render(x, y, nx);
				}
				//ani = SOPHIA_ANI_RUN_HIGH;
				//animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				ani = SOPHIA_ANI_DOWN;
				if (!animations[ani]->IsCompleted())
				{
					animations[ani]->Render(x, y, nx);
					//ani = SOPHIA_ANI_RUN_HIGH;
					//animations[ani]->RenderFrame(currentWalkingColumn, x, y, nx);
				}
				else
				{
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
		}
	}
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