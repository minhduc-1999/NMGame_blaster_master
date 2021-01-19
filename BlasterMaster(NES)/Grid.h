#pragma once
#include "StaticGameObject.h"
#include "DynamicGameObject.h"
#include "Sophia.h"

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
	vector<LPDYNAMICOBJECT>* Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, float xMain, float yMain);
	vector<LPGAMEOBJECT>* GetcoObjectList();
	void Render();
	void Clear();
	vector<LPDYNAMICOBJECT>* GetDynamicObj() {
		vector<LPDYNAMICOBJECT>* temp = new vector<LPDYNAMICOBJECT>;
		temp->insert(temp->end(), dynamicObjs.begin(), dynamicObjs.end());
		return temp;
	}
	Sophia* GetSophia();
};

typedef Grid* LPGRID;

