#pragma once
#include "Section.h"
class SectionArea : public Section
{
public:
	void Load(SaveData* data);
	SectionArea(int id, string path) : Section(id, path) {}
};