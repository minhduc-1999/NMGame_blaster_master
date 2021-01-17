#pragma once
#include "DynamicGameObject.h"

#define ITEM_HP_ANISET	26

class Item
	: public CDynamicGameObject
{
public:
	Item(float x, float y);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetIsDestroyed() { isDestroyed = true; }
};
