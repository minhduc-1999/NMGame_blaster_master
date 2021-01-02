#include "MiniJason.h"
#include "Sophia.h"
#include "CGate.h"
#include "CGate.h"

int currentRunningColumn = 0;

MiniJason::MiniJason(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(MINIJASON_WIDTH, MINIJASON_HEIGHT);
	SophiaBound.left = 0;
	SophiaBound.top = 0;
	SophiaBound.right = 0;
	SophiaBound.bottom = 0;

}

void MiniJason::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	vy += MINIJASON_GRAVITY;

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
		// block 
		//x += min_tx * dx + ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + nty * 0.4f;

		//if (ntx != 0)
		//{
		//	if (GetNX() == 1)
		//	{
		//		SetState(MINIJASON_STATE_IDLE_RIGHT);

		//	}
		//	else
		//	{
		//		SetState(MINIJASON_STATE_IDLE_LEFT);
		//	}
		//}

		//if (nty != 0)
		//{
		//	if(nty == -1)
		//		SetIsJumping(false);
		//	vy = 0;
		//}


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
				SophiaBound = e->obj->GetBound();
				break;
			case 13:
				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
					y += dy;
				break;
			case 15:
				if (e->nx != 0)
					x += e->t * dx + e->nx * 0.4f;
				else if (e->ny != 0)
				{
					y += e->t * dy + e->ny * 0.4f;
					if (e->ny == -1)
						SetIsJumping(false);
					vy = 0;
				}
				break;
			default:
				break;
			};


			//CGate* gate = dynamic_cast<CGate*>(e->obj);
			//if (gate != 0)
			//{
			//	CGame::GetInstance()->SwitchSection(gate->GetNextSectionID(),
			//		gate->GetDesTelePos());
			//	break;
			//	//DebugOut("[Last update normal player pos]\tx: %f, y: %f\n", x, y);
			//}
			/*if (e->obj->GetType() == 1)
			{
				if (ntx != 0)
					x += (1 - min_tx) * dx - ntx * 0.4f;
				else
					y += (1 - min_ty) * dy - nty * 0.4f;
			}
			else if (e->obj->GetType() == 13)
			{
				if (ntx != 0)
					x += (1 - min_tx) * dx - ntx * 0.4f;
				else
					y += (1 - min_ty) * dy - nty * 0.4f;
			}*/

		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	DebugOut("[Last update normal player pos]\tx: %f, y: %f\n", x, y);
}

void MiniJason::Render()
{
	int ani = MINIJASON_ANI_IDLE;
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
				animation_set->at(ani)->RenderFrame(currentRunningColumn, x, y, nx);
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
				animation_set->at(ani)->RenderStartByFrame(currentRunningColumn, x, y, nx);
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
				animation_set->at(ani)->Render(x, y, nx);
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
				animation_set->at(ani)->RenderStartByFrame(currentRunningColumn, x, y, nx);
				return;
				break;
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
		break;
	}
}

bool MiniJason::IsCollisionWithSophia()
{
	if (SophiaBound.left != 0 && SophiaBound.top != 0 && SophiaBound.right != 0 && SophiaBound.bottom != 0)
	{
		Rect miniJasonBound = GetBound();
		if (miniJasonBound.right <= SophiaBound.right && miniJasonBound.left >= SophiaBound.left
			&& miniJasonBound.bottom <= SophiaBound.bottom && miniJasonBound.top >= SophiaBound.top)
		{
			return true;
		}
	}
	
	return false;
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
	case DIK_C:
		SetState(MINIJASON_STATE_TRANSFORM);
		break;
	}
}

void MiniJason::OnKeyUp(int KeyCode)
{
}
