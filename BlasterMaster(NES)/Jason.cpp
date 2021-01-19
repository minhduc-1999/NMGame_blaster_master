#include "Jason.h"
#include "CGate.h"
#include "SceneGate.h"
#include "JasonBullet.h"
#include "Item.h"

Jason::Jason(float x, float y) :MainPlayer(x, y)
{
	SetSize(JASON_WIDTH, JASON_HEIGHT);
	SetHPMAX(16);
	ny = 1;
	lastShot = GetTickCount64();
	canGoArea = false;
	canShoot = true;
	HP = 16;
	SetType(3);
	WinnedBoss = false;
}

int Jason::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == JASON_STATE_DIE)
	{
		if (isDestroyed)
		{
			Sound::getInstance()->stop("lvl2");
			CGame::GetInstance()->Notify(lives);
			return 1;
		}
		return 0;
	}
	
	CDynamicGameObject::Update(dt);
	if (!CanTouch && GetTickCount64() - TouchTime >= 500)
	{
		CanTouch = true;
	}
	isCollisionWithEnemy = false;
	vector< LPCOLLISIONEVENT> curCoEvents;
	CalcNowCollisions(coObjects, curCoEvents);
	if (curCoEvents.size() == 0)
		canGoArea = true;
	for (int i = 0; i < curCoEvents.size(); i++)
	{
		LPGAMEOBJECT temp = curCoEvents[i]->obj;
		if (temp->GetTeam() != this->team)
		{
			isCollisionWithEnemy = true;
		}

		if (temp->GetType() == 26)
		{
			CDynamicGameObject* itemHP = dynamic_cast<CDynamicGameObject*>(temp);
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
				if (this->nx == gate->GetDirectionX() || this->ny == gate->GetDirectionY())
				{
					CGame::GetInstance()->SwitchSection(gate->GetNextSectionID(),
						gate->GetDesTelePos());
					return 1;
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
			Sound::getInstance()->play("JHit", false, 1);
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
		float min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty, min_tbx, min_tby, nbx, nby);

		// block 
		if (nbx != 0)
			x += min_tbx * dx + nbx * 0.4f;
		else
			x += dx;
		//x += min_tx * dx + ntx * 0.4f;
		if (nby != 0)
			y += min_tby * dy + nby * 0.4f;
		else
			y += dy;

		//TODO: Collision logic with dynamic object (bots)
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int objType = e->obj->GetType();
			/*if (objType != 15)
			{
				if (e->nx != 0)
				{
					x += (1 - e->t) * dx - e->nx * 0.4f;
				}
				else
				{
					y += (1 - e->t) * dy - e->ny * 0.4f;
				}
			}*/
		}

	}

	if (HP <= 0)
	{
		Sound::getInstance()->play("JasonDed", false, 1);
		SetState(JASON_STATE_DIE);
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	return 0;
}

void Jason::Render()
{
	int origin = -1;
	int ani = JASON_ANI_IDLE_BOTTOM;
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
	if (GetState() == JASON_STATE_DIE)
	{
		ani = JASON_ANI_DIE;
		if (!animation_set->at(JASON_ANI_DIE)->IsCompleted())
		{
			animation_set->at(JASON_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(JASON_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			//CGame::GetInstance()->SwitchScene(3, 1);
			return;
		}
	}

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
		Sound::getInstance()->stop("Hit");
		break;
	}
	animation_set->at(ani)->Render(x, y, origin, alpha);
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
		//vy = 0;
		nx = -1;
		ny = 0;
		break;
	case JASON_STATE_RUN_RIGHT:
		vx = JASON_SPEED;
		//vy = 0;
		nx = 1;
		ny = 0;
		break;
	case JASON_STATE_RUN_TOP:
		//vx = 0;
		vy = -JASON_SPEED;
		nx = 0;
		ny = -1;
		break;
	case JASON_STATE_RUN_BOTTOM:
		//vx = 0;
		vy = JASON_SPEED;
		nx = 0;
		ny = 1;
		break;
	case JASON_STATE_DIE:
		lives--;
		vx = 0;
		vy = 0;
		break;
	}
}

void Jason::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (state == JASON_STATE_DIE) return; //die
	SetState(JASON_STATE_IDLE);
	if (game->IsKeyDown(DIK_RIGHT))
	{
		SetState(JASON_STATE_RUN_RIGHT);

	}
	if (game->IsKeyDown(DIK_LEFT))
	{
		SetState(JASON_STATE_RUN_LEFT);

	}
	if (game->IsKeyDown(DIK_UP))
	{
		SetState(JASON_STATE_RUN_TOP);

	}
	if (game->IsKeyDown(DIK_DOWN))
	{
		SetState(JASON_STATE_RUN_BOTTOM);

	}
}

void Jason::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_Z:
	{
		if (GetTickCount64() - lastShot >= JASON_SHOOTING_DELAY)
		{
			canShoot = true;
			Sound::getInstance()->play("JasonFire", false, 1);
			lastShot = GetTickCount64();			
		}

		else
			canShoot = false;
		break;
	}
	default:
		break;
	}
}

void Jason::OnKeyUp(int KeyCode)
{

}

BaseBullet* Jason::Shoot()
{
	JasonBullet* bullet = NULL;
	if (canShoot)
	{
		bullet = new JasonBullet(x, y, 0, nx, ny);
	}
	return bullet;
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