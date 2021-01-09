#include "OvwSectionTransition.h"
#include "Jason.h"

void OvwSectionTransition::Update(DWORD dt)
{
	CDynamicGameObject* player = sectionDes->GetPlayer();
	//DebugOut("[Trans Update]\t%s", player);
	int nx = player->GetNX();
	int ny = player->GetNY();
	D3DXVECTOR2 pos = player->GetPosition();

	if (ny == 0)
	{
		if ((nx == 1 && pos.x >= telePos.x) || (nx == -1 && pos.x <= telePos.x))
		{
			_isFinish = true;
		}
	}
	else
	{
		if ((ny == 1 && pos.y >= telePos.y) || (ny == -1 && pos.y <= telePos.y))
		{
			_isFinish = true;
		}
	}
	//DebugOut("[nx player trans]\tDir: %d, dt: %d\n", nx, dt);
	if (ny == 0)
	{
		if (nx == -1)
			player->SetState(JASON_STATE_RUN_LEFT);
		else
			player->SetState(JASON_STATE_RUN_RIGHT);
		player->SetSpeed(SWITCH_SECTION_SPEED * nx, 0.0f);

	}
	else
	{
		if (ny == -1)
			player->SetState(JASON_STATE_RUN_TOP);
		else
			player->SetState(JASON_STATE_RUN_BOTTOM);
		player->SetSpeed(0.0f, SWITCH_SECTION_SPEED * ny);

	}

	player->Update(dt);
	//DebugOut("[Pos player trans]\tx: %f, y: %f\n", pos.x, pos.y);
	CGame::GetInstance()->UpdateSwitchSectionCamera(dt, nx, ny, telePos, sectionDes->GetSectionMapPos(), sectionDes->GetSectionMapDimension());
	

}
