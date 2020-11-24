#pragma once
#include "DynamicGameObject.h"
#include "Grid.h"
class Section
{
private:
	int id;
	string secFilePath;

	CDynamicGameObject* mainPlayer;
	float gridWidth;
	float gridHeight;
	int gridCol, gridRow;
	float mapX, mapY, mapWidth, mapHeight;

	unordered_map<int, LPGRID> grids;

	void _ParseSection_DYNAMIC_OBJECTS(string line);
	void _ParseSection_STATIC_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_GRID(string line);
public:
	CDynamicGameObject* GetPlayer() { return mainPlayer; }
	Section(int id, string path) { this->id = id, this->secFilePath = path; }
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void SetMainPlayer(CDynamicGameObject* player) { mainPlayer = player; }
	vector<int> GetBoundGrid(Rect bound);
	D3DXVECTOR2 GetSectionMapPos() { return D3DXVECTOR2{ mapX, mapY }; }
	D3DXVECTOR2 GetSectionMapDimension() { return D3DXVECTOR2{ mapWidth, mapHeight }; }
};
typedef Section* LPSECTION;
