#pragma once
#include "Section.h"

#define SWITCH_SECTION_SPEED 0.07f

class SectionTransition
{
protected:
	LPSECTION sectionSrc, sectionDes;
	D3DXVECTOR2 telePos;
	bool _isFinish;
public:
	SectionTransition() { _isFinish = false; }
	void Reset() { _isFinish = false; }
	virtual bool IsFinish() { return _isFinish; }
	int GetNextSectionId() { return sectionDes->GetId(); }
	virtual void Update(DWORD dt) = 0;
	virtual void DoAfterSetsection();
	void Setsection(LPSECTION src, LPSECTION des, D3DXVECTOR2 tlPos);
};

typedef SectionTransition* LPSECTRANSITION;

