#include "AreaSectionTransition.h"
#include "Sophia.h"

void AreaSectionTransition::Update(DWORD dt)
{
	CDynamicGameObject* player = sectionDes->GetPlayer();
	//DebugOut("[Trans Update]\t%s", player);
	int nx = player->GetNX();
	//DebugOut("[nx player trans]\tDir: %d, dt: %d\n", nx, dt);
	if (nx == -1)
		player->SetState(SOPHIA_STATE_RUN_LEFT);
	else
		player->SetState(SOPHIA_STATE_RUN_RIGHT);
	player->SetSpeed(SWITCH_SECTION_SPEED * nx, 0.0f);
	player->Update(dt);
	D3DXVECTOR2 pos = player->GetPosition();
	//DebugOut("[Pos player trans]\tx: %f, y: %f\n", pos.x, pos.y);
	CGame::GetInstance()->UpdateSwitchSectionCamera(dt, nx, 0, telePos, sectionDes->GetSectionMapPos(), sectionDes->GetSectionMapDimension());
	if ((nx == 1 && pos.x >= telePos.x) || (nx == -1 && pos.x <= telePos.x))
	{
		_isFinish = true;
	}
}