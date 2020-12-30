#include "Grid.h"
#include "Utils.h"
#include "Floater2.h"
#include "Eyeball.h"

void Grid::AddStaticObj(LPSTATICOBJECT obj)
{
	staticObjs.push_back(obj);
}

void Grid::AddDynamicObj(LPDYNAMICOBJECT obj)
{
	dynamicObjs.push_back(obj);
}

vector<LPDYNAMICOBJECT>* Grid::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, float xMain, float yMain)
{
	vector<LPDYNAMICOBJECT>* res = new vector<LPDYNAMICOBJECT>;
	int i = 0;
	while (i < dynamicObjs.size())
	{
		LPDYNAMICOBJECT temp = dynamicObjs[i];
		if (temp->GetType() == 6)
		{
			Floater2* floater = dynamic_cast<Floater2*>(temp);
			floater->Update(dt, coObjects, xMain, yMain);
		}
		else if (temp->GetType() == 14)
		{
			Eyeball* eyeball = dynamic_cast<Eyeball*>(temp);
			eyeball->Update(dt, coObjects, xMain, yMain);
		}
		else
		{
			temp->Update(dt, coObjects);
		}
		if (!CheckIfBound(this->bound, temp->GetBound()))
		{
			res->push_back(temp);
			dynamicObjs.erase(dynamicObjs.begin() + i);
			continue;
		}
		i++;
	}
	/*for (int i = 0; i < dynamicObjs.size(); i++)
	{
		LPDYNAMICOBJECT temp = dynamicObjs[i];
		temp->Update(dt, coObjects);
		if (!CheckIfBound(this->bound, temp->GetBound()))
		{
			res->push_back(temp);
			dynamicObjs.erase(dynamicObjs.begin() + i);
		}
	}*/
	return res;
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
	vector<LPSTATICOBJECT>::iterator temp = staticObjs.begin();
	while (temp != staticObjs.end())
	{
		delete* temp;
		temp = staticObjs.erase(staticObjs.begin());
	}
	vector<LPDYNAMICOBJECT>::iterator temp2 = dynamicObjs.begin();
	while (temp2 != dynamicObjs.end())
	{
		delete* temp2;
		temp2 = dynamicObjs.erase(dynamicObjs.begin());
	}
}

