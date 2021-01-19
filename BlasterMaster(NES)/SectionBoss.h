#pragma once
#include "Section.h"
class SectionBoss : public Section
{
private:
	DWORD startTime;
	int period;
public:
	int GetPeriod() { return period; }
	virtual int Update(DWORD dt);
	virtual void Render();
	void Load(SaveData* data, D3DXVECTOR2 mainPos);
	SectionBoss(int id, string path) : Section(id, path) { startTime = 0; period = 0; }
};