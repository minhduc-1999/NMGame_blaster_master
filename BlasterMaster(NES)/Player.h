#pragma once

#include "DynamicGameObject.h"



class Player : public CDynamicGameObject
{
public:
	Player(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
};