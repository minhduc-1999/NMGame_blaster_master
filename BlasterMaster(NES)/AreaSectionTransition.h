#pragma once
#include "SectionTransition.h"
class AreaSectionTransition : public SectionTransition
{
public:
	AreaSectionTransition() : SectionTransition() {}
	virtual void Update(DWORD dt);
};