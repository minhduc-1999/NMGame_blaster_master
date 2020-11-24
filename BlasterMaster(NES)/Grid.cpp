#include "Grid.h"

void Grid::AddStaticObj(LPSTATICOBJECT obj)
{
	staticObjs.push_back(obj);
}

void Grid::AddDynamicObj(LPDYNAMICOBJECT obj)
{
	dynamicObjs.push_back(obj);
}

void Grid::RemoveObject(LPSTATICOBJECT obj)
{
	auto it = find(staticObjs.begin(), staticObjs.end(), obj);
	if (it != staticObjs.end())
		staticObjs.erase(it);
}

void Grid::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < dynamicObjs.size(); i++)
	{
		dynamicObjs[i]->Update(dt, coObjects);
	}
}

vector<LPGAMEOBJECT>* Grid::GetcoObjectList()
{
	vector<LPGAMEOBJECT>* temp = new vector<LPGAMEOBJECT>;
	temp->insert(temp->end(), staticObjs.begin(), staticObjs.end());
	temp->insert(temp->end(), dynamicObjs.begin(), dynamicObjs.end());
	return temp;
}

void Grid::Render()
{
	for (int i = 0; i < staticObjs.size(); i++)
	{
		staticObjs[i]->Render();
	}
	for (int i = 0; i < dynamicObjs.size(); i++)
	{
		dynamicObjs[i]->Render();
	}
}

void Grid::Clear()
{
	
}
