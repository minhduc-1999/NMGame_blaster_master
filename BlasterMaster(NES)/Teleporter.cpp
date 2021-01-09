#include "Teleporter.h"
#include <cmath>

Teleporter::Teleporter(float x, float y) :CDynamicGameObject(x, y)
{
	SetSize(24, 32);
}

void Teleporter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CDynamicGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
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
					x = x + rand() % 20 - TELEPORTER_TELE_RANGE;
					y = y + rand() % 20 - TELEPORTER_TELE_RANGE;
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
}

void Teleporter::Render()
{
	int ani = TELEPORTER_ANI_GRAY;
	
	if (GetState() == TELEPORTER_STATE_DIE)
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
