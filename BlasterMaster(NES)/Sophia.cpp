#include "Sophia.h"
#include "CGate.h"
#include "Jumper2.h"
#include "Brick.h"
int lastHeight = 0;
int currentWalkingColumn = 0;


Sophia::Sophia(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(SOPHIA_WIDTH, SOPHIA_HEIGHT);
	heightLevel = SOPHIA_HEIGHT_HIGH;
};

void Sophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vy += SOPHIA_GRAVITY;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (coObjects != NULL)
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
			//SetIsJumping(false);
			vy = 0;
		}

		for (UINT i = 0; i < coEvents.size(); i++)
		{
			if (coEvents[i]->ny < 0)
			{
				SetIsJumping(false);
				vy = 0;
			}
		}
		//TODO: Collision logic with dynamic object (bots)
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			// if e->obj is Gate 
			CGate* gate = dynamic_cast<CGate*>(e->obj);
			if (gate != 0)
			{
				CGame::GetInstance()->SwitchSection(gate->GetNextSectionID(),
					gate->GetDesTelePos());
				break;
				//DebugOut("[Last update normal player pos]\tx: %f, y: %f\n", x, y);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut("[UPDATE SOPHIA]\t%f\t%f\n", x, y);
}

void Sophia::Render()
{
	int ani = -1;

	if (GetIsJumping())
	{
		if (GetIsUp())
		{
			animation_set->at(SOPHIA_ANI_DOWN)->ResetAnim();
			switch (state)
			{
			case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT:case SOPHIA_STATE_FIRING_UP_LEFT:case SOPHIA_STATE_FIRING_UP_RIGHT:
				if (vy < 0)
				{
					ani = SOPHIA_ANI_UP_JUMP;
				}
				else
				{
					ani = SOPHIA_ANI_UP_RUN_LOW;
				}
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx);
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
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx);
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
				animation_set->at(ani)->RenderStartByFrame(currentWalkingColumn, x, y - 8, nx);
				return;
				break;
			}
		}
		else
		{
			animation_set->at(SOPHIA_ANI_UP)->ResetAnim();
			switch (state)
			{
			case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT:case SOPHIA_STATE_FIRING_LEFT:case SOPHIA_STATE_FIRING_RIGHT:
				if (vy < 0)
				{
					ani = SOPHIA_ANI_JUMP_UP;
				}
				else
				{
					ani = SOPHIA_ANI_JUMP_DOWN;
				}
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx);
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
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx);
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
				animation_set->at(ani)->RenderStartByFrame(currentWalkingColumn, x, y - 8, nx);
				return;
				break;
			case SOPHIA_STATE_TURN_RUN:
				ani = SOPHIA_ANI_TURN_JUMP;
				animation_set->at(ani)->Render(x, y - 8, nx);
				if (animation_set->at(ani)->IsCompleted())
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
		if (state == SOPHIA_STATE_TRANSFORM)
		{
			ani = SOPHIA_ANI_TRANSFORM;
			animation_set->at(ani)->Render(x, y - 8, -nx);
			if (animation_set->at(ani)->IsCompleted())
			{
				SetIsUp(false);
				animation_set->at(SOPHIA_ANI_DOWN)->SetIsCompleted();
				animation_set->at(ani)->ResetAnim();
				if (GetNX() == 1)
				{
					SetState(SOPHIA_STATE_IDLE_RIGHT);
				}
				else
				{
					SetState(SOPHIA_STATE_IDLE_LEFT);
				}
			}
		}

		if (GetIsUp())
		{
			animation_set->at(SOPHIA_ANI_DOWN)->ResetAnim();
			switch (state)
			{
			case SOPHIA_STATE_IDLE_RIGHT: case SOPHIA_STATE_IDLE_LEFT: case SOPHIA_STATE_FIRING_UP_LEFT:case SOPHIA_STATE_FIRING_UP_RIGHT:
				ani = SOPHIA_ANI_UP;
				if (animation_set->at(ani)->IsCompleted())
				{
					ani = SOPHIA_ANI_UP_RUN_HIGH;
					animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx);
				}
				else
				{
					animation_set->at(ani)->Render(x, y - 8, nx);
				}
				//ani = SOPHIA_ANI_UP_RUN_HIGH;
				//animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;
			case SOPHIA_STATE_JUMP_RIGHT: case SOPHIA_STATE_JUMP_LEFT:
				/*ani = SOPHIA_ANI_UP;
				if (animation_set->at(ani)->IsCompleted())
				{
					ani = SOPHIA_ANI_UP_RUN_HIGH;
					animation_set->at(ani)->RenderFrame(currentWalkingColumn,x, y, nx);
				}
				else
				{
					animation_set->at(ani)->Render(x, y, nx);
				}*/
				ani = SOPHIA_ANI_UP_RUN_HIGH;
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx);
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				if (heightLevel == SOPHIA_HEIGHT_HIGH)
				{
					lastHeight = 0;
					ani = SOPHIA_ANI_UP_RUN_HIGH;
					heightLevel++;
				}
				else if (heightLevel == SOPHIA_HEIGHT_LOW)
				{
					lastHeight = 2;
					ani = SOPHIA_ANI_UP_RUN_LOW;
					heightLevel--;
				}
				else
				{
					if (lastHeight == 0)
					{
						ani = SOPHIA_ANI_UP_RUN_HIGH;
						heightLevel++;
					}
					else
					{
						ani = SOPHIA_ANI_UP_RUN_LOW;
						heightLevel--;
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

				animation_set->at(ani)->RenderStartByFrame(currentWalkingColumn, x, y - 8, nx);
				return;
				break;
				/*case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT:
					ani = SOPHIA_ANI_UP_RUN_HIGH;
					animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y, nx);
					return;
					break;*/
			}
		}
		else
		{
			animation_set->at(SOPHIA_ANI_UP)->ResetAnim();
			switch (state)
			{
			case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT: case SOPHIA_STATE_FIRING_LEFT: case SOPHIA_STATE_FIRING_RIGHT:
				ani = SOPHIA_ANI_DOWN;
				if (animation_set->at(ani)->IsCompleted())
				{
					ani = SOPHIA_ANI_RUN_HIGH;
					animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx);
				}
				else
				{
					animation_set->at(ani)->Render(x, y - 8, nx);
				}
				//ani = SOPHIA_ANI_RUN_HIGH;
				//animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				ani = SOPHIA_ANI_DOWN;
				if (!animation_set->at(ani)->IsCompleted())
				{
					animation_set->at(ani)->Render(x, y - 8, nx);
					//ani = SOPHIA_ANI_RUN_HIGH;
					//animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y, nx);
				}
				else
				{
					if (heightLevel == SOPHIA_HEIGHT_HIGH)
					{
						lastHeight = 0;
						ani = SOPHIA_ANI_RUN_HIGH;
						heightLevel++;
					}
					else if (heightLevel == SOPHIA_HEIGHT_LOW)
					{
						lastHeight = 2;
						ani = SOPHIA_ANI_RUN_LOW;
						heightLevel--;
					}
					else
					{
						if (lastHeight == 0)
						{
							ani = SOPHIA_ANI_RUN_HIGH;
							heightLevel++;
						}
						else
						{
							ani = SOPHIA_ANI_RUN_LOW;
							heightLevel--;
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

				animation_set->at(ani)->RenderStartByFrame(currentWalkingColumn, x, y - 8, nx);
				return;
				break;
			case SOPHIA_STATE_TURN_RUN:
				ani = SOPHIA_ANI_TURN_RUN;
				animation_set->at(ani)->Render(x, y - 8, nx);
				if (animation_set->at(ani)->IsCompleted())
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

void Sophia::SetState(int state)
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
		vx = SOPHIA_RUN_SPEED;
		nx = 1;
		break;
	case SOPHIA_STATE_RUN_LEFT:
		vx = -SOPHIA_RUN_SPEED;
		nx = -1;
		break;
	case SOPHIA_STATE_JUMP_RIGHT:
		vy = -SOPHIA_JUMP_SPEED_Y;
		nx = 1;
		break;
	case SOPHIA_STATE_JUMP_LEFT:
		vy = -SOPHIA_JUMP_SPEED_Y;
		nx = -1;
		break;
	case SOPHIA_STATE_TURN_RUN:
		break;
	case SOPHIA_STATE_TRANSFORM:
		vx = 0;
		break;
	}
}


void Sophia::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (GetState() == -1) return; //die
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (GetIsUp())
		{
			SetState(SOPHIA_STATE_RUN_RIGHT);
		}
		else
		{
			if (GetNX() == -1)
			{
				SetState(SOPHIA_STATE_TURN_RUN);
			}
			else
			{
				SetState(SOPHIA_STATE_RUN_RIGHT);
			}
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (GetIsUp())
		{
			SetState(SOPHIA_STATE_RUN_LEFT);
		}
		else
		{
			if (GetNX() == 1)
			{
				SetState(SOPHIA_STATE_TURN_RUN);
			}
			else
			{
				SetState(SOPHIA_STATE_RUN_LEFT);
			}
		}
	}
	else
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

	if (game->IsKeyDown(DIK_UP))
	{
		if (GetIsUp() == false)
		{
			SetIsUp(true);
		}
	}
}

void Sophia::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_X:
		if (GetIsJumping() == false)
		{
			SetIsJumping(true);
			if (GetNX() == 1)
			{
				SetState(SOPHIA_STATE_JUMP_RIGHT);
			}
			else
			{
				SetState(SOPHIA_STATE_JUMP_LEFT);
			}
		}
		break;
	case DIK_Z:
		if (GetIsUp())
		{
			if (GetNX() == 1)
				SetState(SOPHIA_STATE_FIRING_UP_RIGHT);
			else
				SetState(SOPHIA_STATE_FIRING_UP_LEFT);
		}
		else
		{
			if (GetNX() == 1)
				SetState(SOPHIA_STATE_FIRING_RIGHT);
			else
				SetState(SOPHIA_STATE_FIRING_LEFT);
		}
		break;
	case DIK_C:
		SetState(SOPHIA_STATE_TRANSFORM);
		break;
	}
}

void Sophia::OnKeyUp(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_UP:
		SetIsUp(false);
		break;
	case DIK_RIGHT: case DIK_LEFT:
		if (GetNX() == 1)
		{
			SetState(SOPHIA_STATE_IDLE_RIGHT);
		}
		else
		{
			SetState(SOPHIA_STATE_IDLE_LEFT);
		}
		break;
	}
}
