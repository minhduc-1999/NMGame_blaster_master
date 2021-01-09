#pragma once
#include "SectionTransition.h"
class OvwSectionTransition :public SectionTransition
{
public:
	OvwSectionTransition() : SectionTransition() {}
	virtual void Update(DWORD dt);
};