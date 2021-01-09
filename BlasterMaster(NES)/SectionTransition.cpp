#include "SectionTransition.h"
#include "Game.h"
#include "DynamicGameObject.h"

#define SWITCH_SECTION_SPEED 0.07f

SectionTransition::SectionTransition()
{
	_isFinish = false;
	sectionDes = NULL;
	sectionSrc = NULL;
	telePos = D3DXVECTOR2(-1, -1);
	saveData = NULL;
}

void SectionTransition::Setsection(LPSECTION src, LPSECTION des, D3DXVECTOR2 tlPos)
{
	sectionSrc = src;
	sectionDes = des;
	telePos = tlPos;
	if (telePos.x == -1 && telePos.y == -1)
	{
		_isFinish = true;
		sectionDes->Load(saveData, tlPos);
		CGame::GetInstance()->UpdateCamera(
			sectionDes->GetPlayer()->GetPosition(),
			sectionDes->GetSectionMapPos(),
			sectionDes->GetSectionMapDimension());
		return;
	}
	//DoAfterSetsection();
	sectionDes->SetMainPlayer(sectionSrc->GetPlayer());
	sectionSrc->Unload();
	sectionDes->Load(saveData, tlPos);
}

void SectionTransition::Setsection(LPSECTION src, LPSECTION des, D3DXVECTOR2 tlPos, SaveData* data)
{
	saveData = data;
	Setsection(src, des, tlPos);
}
