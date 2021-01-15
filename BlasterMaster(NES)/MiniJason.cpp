#include "MiniJason.h"
#include "Sophia.h"
#include "CGate.h"
#include "CGate.h"
#include "CLadder.h"
#include "SceneGate.h"
#include "MiniJasonBullet.h"


MiniJason::MiniJason(float x, float y) :MainPlayer(x, y)
{
	SetSize(MINIJASON_WIDTH, MINIJASON_HEIGHT);
	SetType(2);
	HP = 16;
}

int MiniJason::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);

	if (!CanTouch && GetTickCount64() - TouchTime >= 500)
	{
		CanTouch = true;
	}

	vector< LPCOLLISIONEVENT> curCoEvents;
	isCollisionWithSophia = false;
	isCollisionWithEnemy = false;
	//canGoOvw = false;
	canClimb = false;
	CalcNowCollisions(coObjects, curCoEvents);
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		if (temp->GetTeam() != this->team)
		{
			isCollisionWithEnemy = true;
		}
		switch (temp->GetType())
		{
		case 80:
			if (canGoOvw)
			{ 
				SceneGate* gate = dynamic_cast<SceneGate*>(temp);
				if (gate != 0)
				{
					DebugOut("[To OVW]\tx: %f, y: %f, scene: %d, section: %d\n", gate->GetDesTelePos().x,
						gate->GetDesTelePos().y, gate->GetDesScene(), gate->GetNextSectionID());
					CGame::GetInstance()->SwitchScene(gate->GetDesScene(), gate->GetNextSectionID(), gate->GetDesTelePos());
					return 1;
				}
			}
			//canGoOvw = true;
			//DebugOut("[INFO]\tcan go over world\n");
			break;
		case 17:
		{
			CGate* gate = dynamic_cast<CGate*>(temp);
			if (gate != 0)
			{
				if (this->nx == gate->GetDirectionX())
				{
					//if (abs(this->y - gate->GetDesTelePos().y) < 2)
					//{
					CGame::GetInstance()->SwitchSection(gate->GetNextSectionID(),
						gate->GetDesTelePos());
					return 0;
					//}					
				}
				//DebugOut("[Last update normal player pos]\tx: %f, y: %f\n", x, y);
			}
			break;
		}
		case 1:
			isCollisionWithSophia = true;
			break;
		case 18:
			canClimb = true;
			break;

		case 13:
			isCollisionWithEnemy = true;
			break;
		default:
			break;
		}
	}
	if (isCollisionWithEnemy)
	{
		if (CanTouch)
		{
			CanTouch = false;
			TouchTime = GetTickCount64();
			SetHP(HPDown(HP, 1));
			Sound::getInstance()->play("Hit", false, 1);
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
		float min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby);
		if (nbx != 0)
		{
			x += min_tbx * dx + nbx * 0.4f;
			if (nx == -1)
			{
				SetState(MINIJASON_STATE_IDLE_LEFT);
			}
			else
			{
				SetState(MINIJASON_STATE_IDLE_RIGHT);
			}
		}
		else
			x += dx;
		//x += min_tx * dx + ntx * 0.4f;
		if (nby != 0)
		{
			y += min_tby * dy + nby * 0.4f;
			vy = 0;
			if (nby == -1)
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
		else
			y += dy;

	}
	if (HP <= 0)
	{
		Sound::getInstance()->play("JasonDed", true, 0);
		SetState(MINIJASON_STATE_DIE);
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut("[MINIJASON]\tx: %f, y: %f\n", x, y);
	return 0;
 }

void MiniJason::Render()
{
	int ani = MINIJASON_ANI_IDLE;

	if (GetState() == MINIJASON_STATE_DIE)
	{
		ani = MINIJASON_ANI_DIE;
		if (!animation_set->at(MINIJASON_ANI_DIE)->IsCompleted())
		{
			animation_set->at(MINIJASON_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(MINIJASON_ANI_DIE)->ResetAnim();
			SetState(MINIJASON_STATE_IDLE_RIGHT);
			//CGame::GetInstance()->SwitchScene(3, 1);
			return;
		}
	}

	if (isCollisionWithEnemy)
	{
		if (alpha == 255)
		{
			alpha = 254;
		}
		else
		{
			alpha = 255;
		}
	}
	else
	{
		alpha = 255;
	}
	if (GetState() == MINIJASON_STATE_CLIMB)
	{
		ani = MINIJASON_ANI_CLIMB;
		if (vy == 0)
		{
			animation_set->at(ani)->RenderFrame(0, x, y, nx, alpha);
			return;
		}
	}
	else
	{
		if (GetIsJumping())
		{
			ani = MINIJASON_ANI_JUMP;
			animation_set->at(ani)->Render(x, y, nx, alpha);
		}
		else
		{
			if (GetIsDown())
			{
				switch (state)
				{
				case MINIJASON_STATE_IDLE_RIGHT: case MINIJASON_STATE_IDLE_LEFT:
					ani = MINIJASON_ANI_DOWN_RUN;
					animation_set->at(ani)->RenderFrame(0, x, y, nx, alpha);
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
					animation_set->at(ani)->Render(x, y, nx, alpha);
					break;
				case MINIJASON_STATE_RUN_RIGHT:case MINIJASON_STATE_RUN_LEFT:
					ani = MINIJASON_ANI_RUN;
					break;
				}
			}
		}
	}

	animation_set->at(ani)->Render(x, y, nx, alpha);
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
	case MINIJASON_STATE_DIE:
		vx = 0;
		vy = 0;
		Sound::getInstance()->stop("Hit");
		break;
	}
}


void MiniJason::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (GetState() == MINIJASON_STATE_DIE) return; //die
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
		if (canClimb && GetIsDown() == false)
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
		if (canClimb && GetState() == MINIJASON_STATE_CLIMB)
		{
			vx = 0;
			vy = MINIJASON_CLIMB_SPEED_Y;
		}
		else if (y <= 169 && y >= 143 && x <= 1600 && x >= 1584 && GetState() != MINIJASON_STATE_CLIMB) // standing on top of ladder
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

	if (game->IsKeyDown(DIK_O))
	{
		SetState(MINIJASON_STATE_DIE);
	}
}

void MiniJason::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_DOWN)
		canGoOvw = true;
	else
		canGoOvw = false;
	switch (KeyCode)
	{
	case DIK_Z:
	{
		if (GetTickCount64() - lastShot >= 300)
		{
			canShoot = true;
			Sound::getInstance()->play("JasonFire", false, 1);
			lastShot = GetTickCount64();
		}
		else
			canShoot = false;
		break;
	}
	break;
	case DIK_X:
		if (GetIsDown() == false)
		{
			if (GetIsJumping() == false)
			{
				SetIsJumping(true);
				Sound::getInstance()->play("Jump", false, 1);
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
		/*if (canGoOvw)
		{
			CGame::GetInstance()->SwitchScene(3, 1);
			return;
		}*/
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

BaseBullet* MiniJason::Shoot()
{
	MiniJasonBullet* bullet = NULL;
	if (canShoot)
	{
		bullet = new MiniJasonBullet(x, y, 0, nx, ny);
	}
	return bullet;
}
