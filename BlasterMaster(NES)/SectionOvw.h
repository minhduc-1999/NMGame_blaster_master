#pragma once
#include "Section.h"
class SectionOvw : public Section
{
public:
	void Load(SaveData* data, D3DXVECTOR2 mainPos);
	SectionOvw(int id, string path) : Section(id, path) {}
};