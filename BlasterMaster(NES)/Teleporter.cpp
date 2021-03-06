#include "Teleporter.h"
#include "EyeballBullet.h"
#include <cmath>

Teleporter::Teleporter(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(24, 32);
	startTime = 0;
}

int Teleporter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	startTime += dt;

	if (startTime >= 1200)
	{
		isShooting = true;
		startTime = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		
	}
	else
	{
		float min_tx, min_ty, ntx, nty;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			int coObjType = e->obj->GetType();
			switch (coObjType)
			{
			case 20: //enemy bullet
				if (e->obj->GetTeam() == 0)
				{
					if (GetState()== TELEPORTER_STATE_GREEN)
					SetState(TELEPORTER_STATE_DIE);
				}
				break;
			default:
				break;
			};

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	delay--;
	if (delay < 0)
		if (state == TELEPORTER_STATE_GREEN)
		{
			{
				newx = x + rand() % 100 - TELEPORTER_TELE_RANGE;
				newy = y + rand() % 100 - TELEPORTER_TELE_RANGE;
				Rect nRect;
				nRect.top = newy - height/2;
				nRect.bottom = newy + height / 2;
				nRect.left = newx - width / 2;
				nRect.right = newx + width / 2;

				Rect rect = CGame::GetInstance()->GetCamBound();
				rect.left += 32;
				rect.top += 32;
				rect.right -= 32;
				rect.bottom -= 32;

				while (!(nRect.right<rect.right && nRect.left > rect.left && nRect.top > rect.top && nRect.bottom < rect.bottom))
				{
					newx = x + rand() % 100 - TELEPORTER_TELE_RANGE;
					newy = y + rand() % 100 - TELEPORTER_TELE_RANGE;
					nRect.top = newy - height / 2;
					nRect.bottom = newy + height / 2;
					nRect.left = newx - width / 2;
					nRect.right = newx + width / 2;
				}
				x = newx;
				y = newy;
				Sound::getInstance()->play("Tele", false, 1);
				delay = TELEPORTER_DELAY_TIME;
				swap++;
				if (swap > TELEPORTER_SWAP)
					this->SetState(TELEPORTER_STATE_GRAY);
			}
		}
		else
		{
			delay = TELEPORTER_DELAY_TIME;
			swap++;
			if (swap > TELEPORTER_SWAP)
				this->SetState(TELEPORTER_STATE_GREEN);
		}
	return 0;
}

void Teleporter::Render()
{
	int ani = TELEPORTER_ANI_GRAY;

	if (state == TELEPORTER_ANI_DIE)
	{
		ani = TELEPORTER_ANI_DIE;
		if (!animation_set->at(TELEPORTER_ANI_DIE)->IsCompleted())
		{
			animation_set->at(TELEPORTER_ANI_DIE)->Render(x, y, nx, 255);
			return;
		}
		else
		{
			animation_set->at(TELEPORTER_ANI_DIE)->ResetAnim();
			isDestroyed = true;
			return;
		}
	}

	if (state == TELEPORTER_STATE_GRAY)
	{
		ani = TELEPORTER_ANI_GRAY;
	}

	if (state == TELEPORTER_STATE_GREEN)
	{
		ani = TELEPORTER_ANI_GREEN;
	}


	animation_set->at(ani)->Render(x, y, nx);
}
void Teleporter::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case TELEPORTER_STATE_GRAY:
		isTele = false;
		swap = 0;
		break;
	case TELEPORTER_STATE_GREEN:
		isTele = true;
		swap = 0;
		break;
	case TELEPORTER_STATE_DIE:
		SetSize(0, 0);
		vx = 0;
		vy = 0;
		break;
	}
}

vector<LPDYNAMICOBJECT> Teleporter::Fire(float xMain, float yMain)
{
	vector<LPDYNAMICOBJECT> teleBulls;
	EyeballBullet* bullet = new EyeballBullet(x, y, 1);
	float a = xMain - x;
	float b = yMain - y;
	bullet->SetSpeed(a / sqrt(pow(a, 2) + pow(b, 2)) / 3, b / sqrt(pow(a, 2) + pow(b, 2)) / 3);
	teleBulls.push_back(bullet);
	Sound::getInstance()->play("Cannon", false, 1);
	isShooting = false;

	return teleBulls;
}
