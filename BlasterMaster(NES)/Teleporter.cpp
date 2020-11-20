#include "Teleporter.h"
#include <cmath>

void Teleporter::Update(DWORD dt)
{

	CDynamicGameObject::Update(dt);
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
					this->SetState(TELEPORTER_ANI_GRAY);
			}
		}
		else
		{
			delay = TELEPORTER_DELAY_TIME;
			swap++;
			if (swap > TELEPORTER_SWAP)
				this->SetState(TELEPORTER_ANI_GREEN);
		}

}

void Teleporter::Render()
{
	int ani = TELEPORTER_ANI_GRAY;

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
	}
}
