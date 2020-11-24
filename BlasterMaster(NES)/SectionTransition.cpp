#include "SectionTransition.h"
#include "Game.h"
#include "DynamicGameObject.h"

#define SWITCH_SECTION_SPEED 0.09f

void SectionTransition::Update(DWORD dt)
{
	CDynamicGameObject* player = sectionDes->GetPlayer();
	DebugOut("[Trans Update]\t%s", player);
	int nx = player->GetNX();
	float dx = nx * SWITCH_SECTION_SPEED * dt;
	D3DXVECTOR2 pos = player->GetPosition();
	pos = pos + D3DXVECTOR2(dx, 0);
	player->SetPosition(pos.x, pos.y);
	if ((nx == 1 && pos.x >= telePos.x) || (nx == -1 && pos.x <= telePos.x))
		_isFinish = true;
}

void SectionTransition::DoAfterSetsection()
{
	sectionDes->SetMainPlayer(sectionSrc->GetPlayer());
	sectionSrc->Unload();
	sectionDes->Load();
}

void SectionTransition::Setsection(LPSECTION src, LPSECTION des, D3DXVECTOR2 tlPos)
{
	sectionSrc = src;
	sectionDes = des;
	telePos = tlPos;
	if (telePos.x == -1 && telePos.y == -1)
	{
		_isFinish = true;
		sectionDes->Load();
		return;
	}
	DoAfterSetsection();
}
