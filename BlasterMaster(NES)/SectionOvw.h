#pragma once
#include "Section.h"
class SectionOvw : public Section
{
public:
	void Load(SaveData* data);
	SectionOvw(int id, string path) : Section(id, path) {}
};