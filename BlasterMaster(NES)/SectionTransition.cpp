#include "SectionTransition.h"
#include "Game.h"
#include "DynamicGameObject.h"
#include "Sophia.h"

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
		CGame::GetInstance()->UpdateCamera(
			sectionDes->GetPlayer()->GetPosition(),
			sectionDes->GetSectionMapPos(),
			sectionDes->GetSectionMapDimension());
		return;
	}
	DoAfterSetsection();
}
