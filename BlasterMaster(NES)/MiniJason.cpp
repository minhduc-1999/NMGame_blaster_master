#include "MiniJason.h"

int currentRunningColumn = 0;

MiniJason::MiniJason(float x, float y) :CDynamicGameObject(x, y) 
{
	SetSize(MINIJASON_WIDTH, MINIJASON_HEIGHT);
}

void MiniJason::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vy += MINIJASON_GRAVITY;

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
				SetState(MINIJASON_STATE_IDLE_RIGHT);

			}
			else
			{
				SetState(MINIJASON_STATE_IDLE_LEFT);
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

void MiniJason::Render()
{
	int ani = MINIJASON_ANI_IDLE;
	if (GetIsJumping())
	{
		ani = MINIJASON_ANI_JUMP;
		animations[ani]->Render(x, y, nx);
	}
	else
	{
		if (GetIsDown())
		{
			switch (state)
			{
			case MINIJASON_STATE_IDLE_RIGHT: case MINIJASON_STATE_IDLE_LEFT:
				ani = MINIJASON_ANI_DOWN_RUN;
				animations[ani]->RenderFrame(currentRunningColumn, x, y, nx);
				return;
				break;
			case MINIJASON_STATE_RUN_RIGHT: case MINIJASON_STATE_RUN_LEFT:
				ani = MINIJASON_ANI_DOWN_RUN;
				if (currentRunningColumn == 1)
				{
					currentRunningColumn = 0;
				}
				else
				{
					currentRunningColumn = 1;
				}
				animations[ani]->RenderStartByFrame(currentRunningColumn, x, y, nx);
				return;
				break;
			}
		}
		else
		{
			switch (state)
			{
			case MINIJASON_STATE_IDLE_RIGHT:case MINIJASON_STATE_IDLE_LEFT:
				ani = MINIJASON_ANI_IDLE;
				animations[ani]->Render(x, y, nx);
				break;
			case MINIJASON_STATE_RUN_RIGHT:case MINIJASON_STATE_RUN_LEFT:
				ani = MINIJASON_ANI_RUN;
				if (currentRunningColumn == 1)
				{
					currentRunningColumn = 0;
				}
				else
				{
					currentRunningColumn = 1;
				}
				animations[ani]->RenderStartByFrame(currentRunningColumn, x, y, nx);
				return;
				break;
			}
		}
	}
	animations[ani]->Render(x, y, nx);
}

void MiniJason::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case MINIJASON_STATE_IDLE_RIGHT:
		vx = 0;
		nx = 1;
		break;
	case MINIJASON_STATE_IDLE_LEFT:
		vx = 0;
		nx = -1;
		break;
	case MINIJASON_STATE_RUN_RIGHT:
		if (GetIsDown())
		{
			vx = MINIJASON_DOWN_RUN_SPEED;
		}
		else
		{
			vx = MINIJASON_RUN_SPEED;
		}
		nx = 1;
		break;
	case MINIJASON_STATE_RUN_LEFT:
		if (GetIsDown())
		{
			vx = -MINIJASON_DOWN_RUN_SPEED;
		}
		else
		{
			vx = -MINIJASON_RUN_SPEED;
		}
		nx = -1;
		break;
	case MINIJASON_STATE_JUMP_RIGHT:
		vy = -MINIJASON_JUMP_SPEED_Y;
		nx = 1;
		break;
	case MINIJASON_STATE_JUMP_LEFT:
		vy = -MINIJASON_JUMP_SPEED_Y;
		nx = -1;
		break;
	case MINIJASON_STATE_CLIMB:
		break;
	}
}

void MiniJason::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (GetState() == -1) return; //die
	if (game->IsKeyDown(DIK_RIGHT))
	{
		SetState(MINIJASON_STATE_RUN_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		SetState(MINIJASON_STATE_RUN_LEFT);
	}
	else
	{
		if (GetNX() == 1)
		{
			SetState(MINIJASON_STATE_IDLE_RIGHT);
		}
		else
		{
			SetState(MINIJASON_STATE_IDLE_LEFT);
		}
	}
}

void MiniJason::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_X:
		if (GetIsDown() == false)
		{
			if (GetIsJumping() == false)
			{
				SetIsJumping(true);
				if (GetNX() == 1)
				{
					SetState(MINIJASON_STATE_JUMP_RIGHT);
				}
				else
				{
					SetState(MINIJASON_STATE_JUMP_LEFT);
				}
			}
		}
		break;
	case DIK_DOWN:
		if (GetIsDown() == false)
		{
			SetIsDown(true);
			SetSize(MINIJASON_DOWN_WIDTH, MINIJASON_DOWN_HEIGHT);
			SetPosition(GetPosition().x, GetPosition().y + 3.5);
		}
		break;
	case DIK_UP:
		if (GetIsDown() == true)
		{
			SetSize(MINIJASON_WIDTH, MINIJASON_HEIGHT);
			SetPosition(GetPosition().x, GetPosition().y - 3.5);
			SetIsDown(false);
		}
		break;
	}
}

void MiniJason::OnKeyUp(int KeyCode)
{
}
