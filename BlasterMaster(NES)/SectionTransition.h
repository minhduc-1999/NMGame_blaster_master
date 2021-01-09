#pragma once
#include "Section.h"
#include "SaveData.h"

#define SWITCH_SECTION_SPEED 0.07f

class SectionTransition
{
protected:
	LPSECTION sectionSrc, sectionDes;
	D3DXVECTOR2 telePos;
	bool _isFinish;
	SaveData* saveData;
public:
	SectionTransition();
	void Reset() { _isFinish = false; }
	bool IsFinish() { return _isFinish; }
	int GetNextSectionId() { return sectionDes->GetId(); }
	virtual void Update(DWORD dt) = 0;
	void Setsection(LPSECTION src, LPSECTION des, D3DXVECTOR2 tlPos);
	void Setsection(LPSECTION src, LPSECTION des, D3DXVECTOR2 tlPos, SaveData* data);
};

typedef SectionTransition* LPSECTRANSITION;

