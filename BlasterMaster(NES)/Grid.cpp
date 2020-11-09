#include "Grid.h"

void Grid::AddObject(LPSTATICOBJECT obj)
{
	staticObjs.push_back(obj);
}

void Grid::AddBackgroundTile(LPBACKGROUND bg)
{
	backgrounds.push_back(bg);
}

void Grid::RemoveObject(LPSTATICOBJECT obj)
{
	auto it = find(staticObjs.begin(), staticObjs.end(), obj);
	if (it != staticObjs.end())
		staticObjs.erase(it);
}

void Grid::Update(DWORD dt)
{
	
}

vector<LPSTATICOBJECT>* Grid::GetcoObjectList()
{
	return &staticObjs;
}

void Grid::Render()
{
	int n = staticObjs.size();
	for (int i = 0; i < n; i++)
	{
		staticObjs[i]->Render();
	}
	for (int i = 0; i < backgrounds.size(); i++)
	{
		backgrounds[i]->Render();
	}
}
