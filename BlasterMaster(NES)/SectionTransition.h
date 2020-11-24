#pragma once
#include "Section.h"
class SectionTransition
{
private:
	LPSECTION sectionSrc, sectionDes;
	D3DXVECTOR2 telePos;
	bool _isFinish;
public:
	SectionTransition() { _isFinish = false; }
	void Reset() { _isFinish = false; }
	virtual bool IsFinish() { return _isFinish; }
	virtual void Update(DWORD dt);
	virtual void DoAfterSetsection();
	void Setsection(LPSECTION src, LPSECTION des, D3DXVECTOR2 tlPos);
};

typedef SectionTransition* LPSECTRANSITION;

