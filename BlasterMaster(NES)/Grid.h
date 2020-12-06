#pragma once
#include "StaticGameObject.h"
#include "DynamicGameObject.h"
class Grid
{
protected:
	int gridID;
	Rect bound;
	vector<LPSTATICOBJECT> staticObjs;
	vector<LPDYNAMICOBJECT> dynamicObjs;
public:
 	void AddStaticObj(LPSTATICOBJECT obj);
	void AddDynamicObj(LPDYNAMICOBJECT obj);
	Grid(int id, Rect b) { gridID = id; bound = b; }
	vector<LPDYNAMICOBJECT>* Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	vector<LPGAMEOBJECT>* GetcoObjectList();
	void Render();
	void Clear();
};

typedef Grid* LPGRID;

