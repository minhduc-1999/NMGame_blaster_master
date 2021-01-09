#pragma once
#include "Section.h"
class SectionArea : public Section
{
public:
	void Load(SaveData* data, D3DXVECTOR2 mainPos);
	SectionArea(int id, string path) : Section(id, path) {}
};