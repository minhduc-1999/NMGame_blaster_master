#pragma once
#include "StaticGameObject.h"
#include "Background.h"
class Grid
{
protected:
	int rowID, colID;
	vector<LPSTATICOBJECT> staticObjs;
	vector<LPBACKGROUND> backgrounds;
public:
	void AddObject(LPSTATICOBJECT obj);
	void AddBackgroundTile(LPBACKGROUND bg);
	Grid(int rowid, int colid) { rowID = rowid; colID = colid; }
	void RemoveObject(LPSTATICOBJECT obj);
	void Update(DWORD dt);
	vector<LPSTATICOBJECT>* GetcoObjectList();
	void Render();
};

typedef Grid* LPGRID;

