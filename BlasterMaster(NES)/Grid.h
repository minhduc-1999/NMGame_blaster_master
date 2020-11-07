#pragma once
#include "StaticGameObject.h"
class Grid
{
protected:
	int rowID, colID;
	vector<LPSTATICOBJECT> staticObjs;
public:
	void AddObject(LPSTATICOBJECT obj);
	Grid(int rowid, int colid) { rowID = rowid; colID = colid; }
	void RemoveObject(LPSTATICOBJECT obj);
	void Update(DWORD dt);
	void Render();
};

typedef Grid* LPGRID;

