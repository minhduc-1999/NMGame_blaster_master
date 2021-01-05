#pragma once
#include "DynamicGameObject.h"
#include "Grid.h"
#include "Bullet.h"
#include "HPBar.h"
#include "SaveData.h"
class Section
{
private:
	int id;
	D3DXVECTOR2 defaultPos;
	string secFilePath;

	CDynamicGameObject* mainPlayer;
	float gridWidth;
	float gridHeight;
	int gridCol, gridRow;
	float mapX, mapY, mapWidth, mapHeight;

	vector<Bullet*> bulletObjs;
	bool canFire;

	unordered_map<int, LPGRID> grids;

	void _ParseSection_DYNAMIC_OBJECTS(string line);
	void _ParseSection_STATIC_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_GRID(string line);
	void _ParseSection_DEFAULT(string line);

public:
	CDynamicGameObject* GetPlayer() { return mainPlayer; }
	Section(int id, string path) { this->id = id, this->secFilePath = path; }
	virtual void Load();
	virtual void Load(SaveData* data);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	int GetId() { return this->id; }
	void SetMainPlayer(CDynamicGameObject* player) { mainPlayer = player; }
	vector<int> GetBoundGrid(Rect bound);
	D3DXVECTOR2 GetSectionMapPos() { return D3DXVECTOR2{ mapX, mapY }; }
	D3DXVECTOR2 GetSectionMapDimension() { return D3DXVECTOR2{ mapWidth, mapHeight }; }
	D3DXVECTOR2 GetDefaultPos() { return defaultPos; }
	void AddMiniJason();
	void DeleteMiniJason();
};
typedef Section* LPSECTION;
