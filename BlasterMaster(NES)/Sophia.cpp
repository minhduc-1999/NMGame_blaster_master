#include "Sophia.h"
#include "CGate.h"
#include "Jumper2.h"
#include "Brick.h"
#include "SophiaBullet.h"

Sophia::Sophia(float x, float y) :MainPlayer(x, y)
{
	SetSize(SOPHIA_WIDTH, SOPHIA_HEIGHT);
	heightLevel = SOPHIA_HEIGHT_HIGH;
	lastFrameChange = GetTickCount64();
	TouchTime = GetTickCount64();
	HP = 16;
	isUp = false;
	currentWalkingColumn = 0;
	lastHeight = 0;
};

int Sophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);
	if (vx != 0)
	{
		DWORD now = GetTickCount64();
		if (now - lastFrameChange >= 20)
		{
			lastFrameChange = now;
			if (currentWalkingColumn == 3)
			{
				currentWalkingColumn = 0;
			}
			else
			{
				currentWalkingColumn++;
			}
		}
	}

	//check now collision
	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	isCollisionWithEnemy = false;
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		DWORD now = GetTickCount64();
		switch (temp->GetType())
		{
		case 13:case 5:
			isCollisionWithEnemy = true;
			if (GetTickCount64() - TouchTime >= 500)
			{
				TouchTime = now;
				SetHP(HPDown(HP, 1));
				Sound::getInstance()->play("Hit", false, 1);
			}
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
		default:
			break;
		}
	}
	for (UINT i = 0; i < curCoEvents.size(); i++) delete curCoEvents[i];

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
		x += min_tx * dx + ntx * 0.4f;
		y += min_ty * dy + nty * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int coObjType = e->obj->GetType();
			if (coObjType != 15)
			{
				if (e->nx != 0)
				{
					x += (1 - e->t) * dx - e->nx * 0.4f;
				}
				else
				{
					y += (1 - e->t) * dy - e->ny * 0.4f;
				}
			}
			switch (coObjType)
			{
			case 15:	//brick
				if (e->nx != 0)
				{
					if (nx == -1)
					{
						SetState(SOPHIA_STATE_IDLE_LEFT);
					}
					else
					{
						SetState(SOPHIA_STATE_IDLE_RIGHT);
					}
					//x += e->t * dx + e->nx * 0.4f;
				}
				if (e->ny != 0)
				{
					vy = 0;
					if (e->ny == -1)
					{
						SetIsJumping(false);
					}
					//y += e->t * dy + e->ny * 0.4f;
				}
				break;
			default:
				break;
			};
		}
	}
	if (HP <= 0)
	{
		Sound::getInstance()->play("SophiaDed", true, 0);
		SetState(SOPHIA_STATE_DIE);
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut("[SOPHIA]\t%f\t%f\n", x, y);
	return 0;
}

void Sophia::Render()
{
	int ani = -1;
	if (GetState() == SOPHIA_STATE_DIE)
	{
		ani = SOPHIA_ANI_DIE;
		if (!animation_set->at(SOPHIA_ANI_DIE)->IsCompleted())
		{
			animation_set->at(SOPHIA_ANI_DIE)->Render(x, y - 8, -1, 255);
			return;
		}
		else
		{
			animation_set->at(SOPHIA_ANI_DIE)->ResetAnim();
			SetState(SOPHIA_STATE_IDLE_RIGHT);
			//CGame::GetInstance()->SwitchScene(2, 1);
			return;
		}
	}

	if (isCollisionWithEnemy)
	{
		DWORD now = GetTickCount64();
		if (GetTickCount64() - TouchTime >= 50)
		{
			TouchTime = now;
			if (alpha == 255)
			{
				alpha = 254;
			}
			else
			{
				alpha = 255;
			}
		}
	}
	else
	{
		alpha = 255;
	}

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
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 25, nx, alpha);
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
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx, alpha);
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				ani = SOPHIA_ANI_UP_JUMP;
				/*if (currentWalkingColumn == 3)
				{
					currentWalkingColumn = 0;
				}
				else
				{
					currentWalkingColumn++;
				}*/
				animation_set->at(ani)->RenderStartByFrame(currentWalkingColumn, x, y - 8, nx, alpha);
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
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx, alpha);
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
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx, alpha);
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
				/*if (currentWalkingColumn == 3)
				{
					currentWalkingColumn = 0;
				}
				else
				{
					currentWalkingColumn++;
				}*/
				animation_set->at(ani)->RenderStartByFrame(currentWalkingColumn, x, y - 8, nx, alpha);
				return;
				break;
			case SOPHIA_STATE_TURN_RUN:
				ani = SOPHIA_ANI_TURN_JUMP;
				animation_set->at(ani)->Render(x, y - 8, nx, alpha);
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
			animation_set->at(ani)->Render(x, y - 8, -nx, alpha);
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
					animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx, alpha);
				}
				else
				{
					animation_set->at(ani)->Render(x, y - 8, nx, alpha);
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
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx, alpha);
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

				/*if (currentWalkingColumn == 3)
				{
					currentWalkingColumn = 0;
				}
				else
				{
					currentWalkingColumn++;
				}*/

				animation_set->at(ani)->RenderStartByFrame(currentWalkingColumn, x, y - 8, nx, alpha);
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
					animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx, alpha);
				}
				else
				{
					animation_set->at(ani)->Render(x, y - 8, nx, alpha);
				}
				//ani = SOPHIA_ANI_RUN_HIGH;
				//animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y, nx);
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				ani = SOPHIA_ANI_DOWN;
				if (!animation_set->at(ani)->IsCompleted())
				{
					animation_set->at(ani)->Render(x, y - 8, nx, alpha);
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
						lastHeight = 3;
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

				/*if (currentWalkingColumn == 3)
				{
					currentWalkingColumn = 0;
				}
				else
				{
					currentWalkingColumn++;
				}*/

				animation_set->at(ani)->RenderStartByFrame(currentWalkingColumn, x, y - 8, nx, alpha);
				return;
				break;
			case SOPHIA_STATE_TURN_RUN:
				ani = SOPHIA_ANI_TURN_RUN;
				animation_set->at(ani)->Render(x, y - 8, nx, alpha);
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
	case SOPHIA_STATE_DIE:
		vx = 0;
		vy = 0;
		Sound::getInstance()->stop("Hit");
		break;
	}
}


void Sophia::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (GetState() == SOPHIA_STATE_DIE) return; //die
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

	if (game->IsKeyDown(DIK_O))
	{
		SetState(SOPHIA_STATE_DIE);
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
			Sound::getInstance()->play("Jump", false, 1);
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
		if (GetTickCount64() - lastShot >= 300)
		{
			canShoot = true;
			lastShot = GetTickCount64();
		}
		else
			canShoot = false;
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

BaseBullet* Sophia::Shoot()
{
	SophiaBullet* bullet = NULL;
	if (canShoot)
	{
		if (GetIsUp())
		{
			bullet = new SophiaBullet(x, y, 0, nx, 1);
		}
		else
		{
			bullet = new SophiaBullet(x, y, 0, nx, ny);
		}
	}
	return bullet;
}
