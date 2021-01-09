#include "Jason.h"
#include "CGate.h"
#include "SceneGate.h"

Jason::Jason(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(JASON_WIDTH, JASON_HEIGHT);
	ny = 1;
}

int Jason::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);
	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	if (curCoEvents.size() == 0)
		canGoArea = true;
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		switch (temp->GetType())
		{
		case 17:
		{
			CGate* gate = dynamic_cast<CGate*>(temp);
			if (gate != 0)
			{
				if (this->nx == gate->GetDirectionX() || this->ny == gate->GetDirectionY())
				{
					CGame::GetInstance()->SwitchSection(gate->GetNextSectionID(),
						gate->GetDesTelePos());
					return 0;
				}
				//DebugOut("[Last update normal player pos]\tx: %f, y: %f\n", x, y);
			}
			break;
		}
		case 80:
			if (canGoArea)
			{
				SceneGate* gate = dynamic_cast<SceneGate*>(temp);
				if (gate != 0)
				{
					DebugOut("[To Area]\tx: %f, y: %f, scene: %d, section: %d\n", gate->GetDesTelePos().x, 
						gate->GetDesTelePos().y, gate->GetDesScene(), gate->GetNextSectionID());
					CGame::GetInstance()->SwitchScene(gate->GetDesScene(), gate->GetNextSectionID(), gate->GetDesTelePos());
					return 1;
				}
			}
			break;
		default:
			canGoArea = true;
			break;
		}
	}
	for (UINT i = 0; i < curCoEvents.size(); i++) delete curCoEvents[i];

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
		x += min_tx * dx + ntx * 0.4f;
		y += min_ty * dy + nty * 0.4f;

		//TODO: Collision logic with dynamic object (bots)
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int objType = e->obj->GetType();
			if (objType != 15)
			{
				if (e->nx != 0)
				{
					x += (1- e->t) * dx - e->nx * 0.4f;		
				}
				else
				{
					y += (1 - e->t) * dy - e->ny * 0.4f;
				}
			}
		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut("[JASON]\tx: %f, y: %f\n", x, y);
	return 0;
}

void Jason::Render()
{
	int origin = -1;
	int ani = JASON_ANI_IDLE_BOTTOM;
	switch (state)
	{
	case JASON_STATE_IDLE:
		if (ny == 0)
		{
			ani = JASON_ANI_IDLE_LEFT;
			if (nx == 1)
			{
				origin = 1;
			}
		}
		else if (ny == 1)
		{
			ani = JASON_ANI_IDLE_BOTTOM;
		}
		else
			ani = JASON_ANI_IDLE_TOP;
		break;
	case JASON_STATE_RUN_LEFT:
		ani = JASON_ANI_RUN_LEFT;
		break;
	case JASON_STATE_RUN_RIGHT:
		ani = JASON_ANI_RUN_LEFT;
		origin = 1;
		break;
	case JASON_STATE_RUN_TOP:
		ani = JASON_ANI_RUN_TOP;
		break;
	case JASON_STATE_RUN_BOTTOM:
		ani = JASON_ANI_RUN_BOTTOM;
		break;
	case JASON_STATE_DIE:
		ani = JASON_ANI_DIE;
		break;
	}
	animation_set->at(ani)->Render(x, y, origin);
}

void Jason::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case JASON_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case JASON_STATE_RUN_LEFT:
		vx = -JASON_SPEED;
		vy = 0;
		nx = -1;
		ny = 0;
		break;
	case JASON_STATE_RUN_RIGHT:
		vx = JASON_SPEED;
		vy = 0;
		nx = 1;
		ny = 0;
		break;
	case JASON_STATE_RUN_TOP:
		vx = 0;
		vy = -JASON_SPEED;
		nx = 0;
		ny = -1;
		break;
	case JASON_STATE_RUN_BOTTOM:
		vx = 0;
		vy = JASON_SPEED;
		nx = 0;
		ny = 1;
		break;
	}
}

void Jason::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (GetState() == -1) return; //die
	if (game->IsKeyDown(DIK_RIGHT))
	{
		SetState(JASON_STATE_RUN_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		SetState(JASON_STATE_RUN_LEFT);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		SetState(JASON_STATE_RUN_TOP);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		SetState(JASON_STATE_RUN_BOTTOM);
	}
	else
	{
		SetState(JASON_STATE_IDLE);
	}
}

void Jason::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
		/*case DIK_Z:
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
			break;*/
	default:
		break;
	}
}

void Jason::OnKeyUp(int KeyCode)
{
}

Rect Jason::GetBound()
{
	Rect bound;

	bound.left = x - width / 2;
	bound.right = bound.left + width - 1;
	bound.top = y + 3;
	bound.bottom = bound.top + 13;

	return bound;
}