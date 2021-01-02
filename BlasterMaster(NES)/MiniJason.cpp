#include "MiniJason.h"
#include "Sophia.h"
#include "CGate.h"
#include "CGate.h"
#include "CLadder.h"

MiniJason::MiniJason(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(MINIJASON_WIDTH, MINIJASON_HEIGHT);
}

void MiniJason::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vector< LPCOLLISIONEVENT> curCoEvents;
	isCollisionWithSophia = false;
	isCollisionWithLadder = false;
	CalcNowCollisions(coObjects, curCoEvents);
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		switch (temp->GetType())
		{
		case 1:
			isCollisionWithSophia = true;
			break;
		case 18:
			isCollisionWithLadder = true;
			break;
		default:
			break;
		}
	}
	for (UINT i = 0; i < curCoEvents.size(); i++) delete curCoEvents[i];

	if (GetState() != MINIJASON_STATE_CLIMB)
	{
		vy += MINIJASON_GRAVITY;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	//No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, ntx, nty;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty);
		if (ntx == 0)
			x += dx;
		if (nty == 0)
			y += dy;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int coObjType = e->obj->GetType();
			switch (coObjType)
			{
			case 1:
				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
					y += dy;
				break;
			case 13:
				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
					y += dy;
				break;
			case 18:	//Ladder
				x += dx;
				if (e->ny != 0)
				{
					if (e->ny == -1 
						&& dynamic_cast<CLadder*>(e->obj)->GetPosition().y == 168.0)
					{
						y += e->t * dy + e->ny * 0.4f;
					}
					else
					{
						y += dy;
					}
				}
				break;
			case 15:
				if (e->nx != 0)
				{
					x += e->t * dx + e->nx * 0.4f;
					if (nx == -1)
					{
						SetState(MINIJASON_STATE_IDLE_LEFT);
					}
					else
					{
						SetState(MINIJASON_STATE_IDLE_RIGHT);
					}
				}
				if (e->ny != 0)
				{
					y += e->t * dy + e->ny * 0.4f;
					vy = 0;
					if (e->ny == -1)
					{
						SetIsJumping(false);
						if (GetState() == MINIJASON_STATE_CLIMB)
						{
							if (nx == 1)
							{
								SetState(MINIJASON_STATE_IDLE_RIGHT);
							}
							else
							{
								SetState(MINIJASON_STATE_IDLE_LEFT);
							}
						}
					}
				}
				break;
			case 79:
				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
					y += dy;
				break;
			case 17:
				CGate * gate = dynamic_cast<CGate*>(e->obj);
				if (gate != 0)
				{
					CGame::GetInstance()->SwitchSection(gate->GetNextSectionID(),
						gate->GetDesTelePos());
					for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
					return;
					//DebugOut("[Last update normal player pos]\tx: %f, y: %f\n", x, y);
				}
			};


			

		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void MiniJason::Render()
{
	int ani = MINIJASON_ANI_IDLE;
	if (GetState() == MINIJASON_STATE_CLIMB)
	{
		ani = MINIJASON_ANI_CLIMB;
		if (vy == 0)
		{
			animation_set->at(ani)->RenderFrame(0, x, y, nx);
			return;
		}
	}
	else
	{
		if (GetIsJumping())
		{
			ani = MINIJASON_ANI_JUMP;
			animation_set->at(ani)->Render(x, y, nx);
		}
		else
		{
			if (GetIsDown())
			{
				switch (state)
				{
				case MINIJASON_STATE_IDLE_RIGHT: case MINIJASON_STATE_IDLE_LEFT:
					ani = MINIJASON_ANI_DOWN_RUN;
					animation_set->at(ani)->RenderFrame(0, x, y, nx);
					return;
					break;
				case MINIJASON_STATE_RUN_RIGHT: case MINIJASON_STATE_RUN_LEFT:
					ani = MINIJASON_ANI_DOWN_RUN;
					break;
				}
			}
			else
			{
				switch (state)
				{
				case MINIJASON_STATE_IDLE_RIGHT:case MINIJASON_STATE_IDLE_LEFT:
					ani = MINIJASON_ANI_IDLE;
					animation_set->at(ani)->Render(x, y, nx);
					break;
				case MINIJASON_STATE_RUN_RIGHT:case MINIJASON_STATE_RUN_LEFT:
					ani = MINIJASON_ANI_RUN;
					break;
				}
			}
		}
	}
	
	animation_set->at(ani)->Render(x, y, nx);
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
	case MINIJASON_STATE_TRANSFORM:
		y -= 15;
		break;
	case MINIJASON_STATE_CLIMB:
		x = 1592.0;
		vy = 0;
		break;
	}
}


void MiniJason::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (GetState() == -1) return; //die
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (GetState() != MINIJASON_STATE_CLIMB)
		{
			SetState(MINIJASON_STATE_RUN_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (GetState() != MINIJASON_STATE_CLIMB)
		{
			SetState(MINIJASON_STATE_RUN_LEFT);
		}
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		if (isCollisionWithLadder && GetIsDown() == false)
		{
			if (GetState() != MINIJASON_STATE_CLIMB)
			{
				SetState(MINIJASON_STATE_CLIMB);
			}
			vx = 0;
			vy = -MINIJASON_CLIMB_SPEED_Y;
		}
		else
		{
			if (GetState() == MINIJASON_STATE_CLIMB)
			{
				y -= 2;
				if (nx == 1)
				{
					SetState(MINIJASON_STATE_IDLE_RIGHT);
				}
				else
				{
					SetState(MINIJASON_STATE_IDLE_LEFT);
				}
			}
		}
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (isCollisionWithLadder && GetState() == MINIJASON_STATE_CLIMB)
		{
			vx = 0;
			vy = MINIJASON_CLIMB_SPEED_Y;
		}
		else if (y <= 169 && y >= 143 && x <= 1600 && x >= 1584  && GetState() != MINIJASON_STATE_CLIMB) // standing on top of ladder
		{
			y += 2;
			SetState(MINIJASON_STATE_CLIMB);
		}
		else
		{
			if (nx == 1)
			{
				SetState(MINIJASON_STATE_IDLE_RIGHT);
			}
			else
			{
				SetState(MINIJASON_STATE_IDLE_LEFT);
			}
		}
	}
	else
	{
		if (GetState() != MINIJASON_STATE_CLIMB)
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
		else
		{
			SetState(MINIJASON_STATE_CLIMB);
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
		if (canGoOvw)
		{
			CGame::GetInstance()->SwitchScene(3);
			return;
		}
		if (GetState() != MINIJASON_STATE_CLIMB)
		{
			if (GetIsDown() == false)
			{
				SetIsDown(true);
				SetSize(MINIJASON_DOWN_WIDTH, MINIJASON_DOWN_HEIGHT);
				SetPosition(GetPosition().x, GetPosition().y + 3.5);
			}
		}
		break;
	case DIK_UP:
		if (GetState() != MINIJASON_STATE_CLIMB)
		{
			if (GetIsDown() == true)
			{
				SetSize(MINIJASON_WIDTH, MINIJASON_HEIGHT);
				SetPosition(GetPosition().x, GetPosition().y - 3.5);
				SetIsDown(false);
			}
		}
		break;
	case DIK_C:
		SetState(MINIJASON_STATE_TRANSFORM);
		break;
	}
}

void MiniJason::OnKeyUp(int KeyCode)
{
}
