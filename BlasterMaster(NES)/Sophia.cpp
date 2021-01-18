#include "Sophia.h"
#include "CGate.h"
#include "Jumper2.h"
#include "Brick.h"
#include "SophiaBullet.h"
#include "Item.h"

Sophia::Sophia(float x, float y) :MainPlayer(x, y)
{
	SetSize(SOPHIA_WIDTH, SOPHIA_HEIGHT);
	SetType(1);
	heightLevel = SOPHIA_HEIGHT_HIGH;
	lastFrameChange = GetTickCount64();
	heightChange = GetTickCount64();
	SetHPMAX(16);
	isUp = false;
	currentWalkingColumn = 0;
	lastHeight = 0;
	currentBullet = BULLET_NORMAL;
};

int Sophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == SOPHIA_STATE_DIE)
	{
		CGame::GetInstance()->SwitchScene(4, 1, D3DXVECTOR2(-1, -1));
		return 1;
	}
	CDynamicGameObject::Update(dt);
	/*if (vx != 0)
	{
		DWORD now = GetTickCount64();
		if (now - lastFrameChange >= 45)
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
	}*/
	if (!CanTouch && GetTickCount64() - TouchTime >= 500)
	{
		CanTouch = true;
	}

	//check now collision
	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	isCollisionWithEnemy = false;
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		//DWORD now = GetTickCount64();
		if (temp->GetTeam() != GetTeam())
		{
			isCollisionWithEnemy = true;
		}

		if (temp->GetType() == 26)
		{
			Item* itemHP = dynamic_cast<Item*>(temp);
			if (!itemHP->GetIsDestroyed())
			{
				HPDown(-1);
			}
			itemHP->SetIsDestroyed();
		}

		switch (temp->GetType())
		{
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
	if (isCollisionWithEnemy)
	{
		if (CanTouch)
		{
			CanTouch = false;
			TouchTime = GetTickCount64();
			HPDown(1);
			Sound::getInstance()->play("Hit", false, 1);
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
		float min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby);
		if (nbx != 0)
			x += min_tbx * dx + nbx * 0.4f;
		else
			x += dx;
			//x += min_tx * dx + ntx * 0.4f;
		if (nby != 0)
		{
			y += min_tby * dy + nby * 0.4f;
			if (nby == -1)
			{
				SetIsJumping(false);
				vy = 0;
			}
			else
				vy = SOPHIA_GRAVITY;
			
		}
		else
			y += dy;
			//y += min_ty * dy + nty * 0.4f;
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
	if (vx != 0)
	{
		DWORD now = GetTickCount64();
		if (now - lastFrameChange >= 45)
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
		if (now - heightChange >= 100)
		{
			heightChange = now;
			if (heightLevel == 1)
			{
				heightLevel--;
			}
			else
			{
				heightLevel++;
			}
			
		}
	}
	int ani = -1;
	if (GetState() == SOPHIA_STATE_DIE && isJumping == false)
	{
		ani = SOPHIA_ANI_DIE;
		if (!animation_set->at(SOPHIA_ANI_DIE)->IsCompleted())
		{
			animation_set->at(SOPHIA_ANI_DIE)->Render(x, y - 15, -1, 255);
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

	if (GetIsJumping())
	{
		if (GetIsUp())
		{
			animation_set->at(SOPHIA_ANI_DOWN)->ResetAnim();
			switch (state)
			{
			case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT:case SOPHIA_STATE_FIRING_UP_LEFT:case SOPHIA_STATE_FIRING_UP_RIGHT:case SOPHIA_STATE_DIE:case SOPHIA_STATE_JUMP_RIGHT:case SOPHIA_STATE_JUMP_LEFT:
				ani = SOPHIA_ANI_UP;
				if (animation_set->at(ani)->IsCompleted())
				{
					if (vy < 0)
					{
						ani = SOPHIA_ANI_UP_JUMP;
					}
					else
					{
						ani = SOPHIA_ANI_UP_RUN_LOW;
					}
					animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx, alpha);
				}
				else
				{
					animation_set->at(ani)->Render(x, y - 8, nx, alpha);
				}
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				ani = SOPHIA_ANI_UP_JUMP;
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
			case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT:case SOPHIA_STATE_FIRING_LEFT:case SOPHIA_STATE_FIRING_RIGHT:case SOPHIA_STATE_DIE:case SOPHIA_STATE_JUMP_RIGHT:case SOPHIA_STATE_JUMP_LEFT:
				ani = SOPHIA_ANI_DOWN;
				if (animation_set->at(ani)->IsCompleted())
				{
					if (vy < 0)
					{
						ani = SOPHIA_ANI_JUMP_UP;
					}
					else
					{
						ani = SOPHIA_ANI_JUMP_DOWN;
					}
					animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx, alpha);
				}
				else
				{
					animation_set->at(ani)->Render(x, y - 8, nx, alpha);
				}
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
				animation_set->at(ani)->RenderStartByFrame(currentWalkingColumn, x, y - 8, nx, alpha);
				return;
				break;
			case SOPHIA_STATE_TURN_RUN:
				ani = SOPHIA_ANI_TURN_JUMP;
				animation_set->at(ani)->Render(x, y - 8, nx, alpha);
				if (animation_set->at(ani)->IsCompleted())
				{
					animation_set->at(ani)->ResetAnim();
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
			case SOPHIA_STATE_IDLE_RIGHT: case SOPHIA_STATE_IDLE_LEFT: case SOPHIA_STATE_FIRING_UP_LEFT:case SOPHIA_STATE_FIRING_UP_RIGHT:case SOPHIA_STATE_DIE:
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
				return;
				break;
			case SOPHIA_STATE_JUMP_RIGHT: case SOPHIA_STATE_JUMP_LEFT:
				ani = SOPHIA_ANI_UP_RUN_HIGH;
				animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y - 8, nx, alpha);
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				if (heightLevel == 0)
				{
					ani = SOPHIA_ANI_UP_RUN_HIGH;
				}
				else
				{
					ani = SOPHIA_ANI_UP_RUN_LOW;
				}
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
			case SOPHIA_STATE_IDLE_RIGHT:case SOPHIA_STATE_IDLE_LEFT: case SOPHIA_STATE_FIRING_LEFT: case SOPHIA_STATE_FIRING_RIGHT:case SOPHIA_STATE_DIE:
				ani = SOPHIA_ANI_DOWN;
				if (animation_set->at(ani)->IsCompleted())
				{
					ani = SOPHIA_ANI_RUN_HIGH;
					animation_set->at(ani)->RenderFrame(currentWalkingColumn, x, y, nx, alpha);
				}
				else
				{
					animation_set->at(ani)->Render(x, y-8, nx, alpha);
				}
				return;
				break;
			case SOPHIA_STATE_RUN_RIGHT:case SOPHIA_STATE_RUN_LEFT:
				ani = SOPHIA_ANI_DOWN;
				if (!animation_set->at(ani)->IsCompleted())
				{
					animation_set->at(ani)->Render(x, y - 8, nx, alpha);
				}
				else
				{
					if (heightLevel == 0)
					{
						ani = SOPHIA_ANI_RUN_HIGH;
					}
					else
					{
						ani = SOPHIA_ANI_RUN_LOW;
					}
					animation_set->at(ani)->RenderStartByFrame(currentWalkingColumn, x, y, nx, alpha);
				}
				return;
				break;
			case SOPHIA_STATE_TURN_RUN:
				ani = SOPHIA_ANI_TURN_RUN;
				animation_set->at(ani)->Render(x, y, nx, alpha);
				if (animation_set->at(ani)->IsCompleted())
				{
					animation_set->at(ani)->ResetAnim();
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
		vx = 0;
		break;
	case SOPHIA_STATE_TRANSFORM:
		vx = 0;
		break;
	case SOPHIA_STATE_DIE:
		vx = 0;
		//vy = 0;
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
		if (GetState() != SOPHIA_STATE_TURN_RUN)
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
			currentBullet = BULLET_NORMAL;
			canShoot = true;
			lastShot = GetTickCount64();
		}
		else
			canShoot = false;
		break;
	case DIK_V:
		if (GetTickCount64() - lastShot >= 300)
		{
			currentBullet = BULLET_ROCKET;
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
		if (GetState() != SOPHIA_STATE_TURN_RUN)
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
		break;
	}
}

vector<LPDYNAMICOBJECT> Sophia::Shoot()
{
	vector<LPDYNAMICOBJECT> bullets;
	if (canShoot)
	{
		if (GetIsUp())
		{
			SophiaBullet* bulletUp = new SophiaBullet(x, y, 0, 0, nx, 1);
			bullets.push_back(bulletUp);
		}
		else
		{
			if (currentBullet == BULLET_NORMAL)
			{
				SophiaBullet* bullet = new SophiaBullet(x, y, 0, 0, nx);
				bullets.push_back(bullet);
			}
			else if (currentBullet == BULLET_ROCKET)
			{
				SophiaBullet* bullet1 = new SophiaBullet(x, y, 0, 1, nx);
				bullet1->SetSpeed(nx * 0.5f, -0.2f);
				bullets.push_back(bullet1);
				SophiaBullet* bullet2 = new SophiaBullet(x, y, 0, 1, nx);
				bullet2->SetSpeed(nx * 0.5f, 0);
				bullets.push_back(bullet2);
				SophiaBullet* bullet3 = new SophiaBullet(x, y, 0, 1, nx);
				bullet3->SetSpeed(nx * 0.5f, 0.2f);
				bullets.push_back(bullet3);
			}
		}
	}
	return bullets;
}
