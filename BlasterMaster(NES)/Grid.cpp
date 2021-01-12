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

Sophia* Grid::GetSophia()
{
	for (int i = 0; i < dynamicObjs.size(); i++)
	{
		LPDYNAMICOBJECT temp = dynamicObjs[i];
		if (temp->GetType() == 1)
		{
			Sophia* sophia = (Sophia*)temp;
			dynamicObjs.erase(dynamicObjs.begin() + i);
			return sophia;
		}
	}

	return NULL;
}

vector<LPDYNAMICOBJECT>* Grid::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, float xMain, float yMain)
{
	vector<LPDYNAMICOBJECT>* res = new vector<LPDYNAMICOBJECT>;
	int i = 0;
	while (i < dynamicObjs.size())
	{
		LPDYNAMICOBJECT temp = dynamicObjs[i];
		
		temp->Update(dt, coObjects);

		if (temp->GetIsShooting())
		{
			vector<LPDYNAMICOBJECT> bullet;
			if (temp->GetType() == 14 || temp->GetType() == 6)
			{
				bullet = temp->Fire(xMain, yMain);
			}
			else
			{
				bullet = temp->Fire();
			}
			if (!bullet.empty())
			{
				dynamicObjs.insert(dynamicObjs.end(), bullet.begin(), bullet.end());
			}
		}

		if (temp->GetIsDestroyed())
		{
			dynamicObjs.erase(dynamicObjs.begin() + i);
			continue;
		}

		if (!CheckIfBound(this->bound, temp->GetBound()))
		{
			res->push_back(temp);
			dynamicObjs.erase(dynamicObjs.begin() + i);
			continue;
		}
		i++;
	}
	/*for (int i = 0; i < bulletObjs.size(); i++)
	{
		bulletObjs[i]->Update(dt, coObjects);
		if (bulletObjs[i]->GetIsDestroyed())
		{
			delete bulletObjs[i];
			bulletObjs.erase(bulletObjs.begin() + i);
		}
	}*/
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
	/*for (int i = 0; i < bulletObjs.size(); i++)
	{
		bulletObjs[i]->Render();
	}*/
}

void Grid::Clear()
{
	/*vector<LPSTATICOBJECT>::iterator temp = staticObjs.begin();
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
	}*/
	for (int i = 0; i < staticObjs.size(); i++)
	{
		delete staticObjs[i];
	}/*
	for (int i = 0; i < dynamicObjs.size(); i++)
	{
		delete dynamicObjs[i];
	}*/
	staticObjs.clear();
	dynamicObjs.clear();
}

