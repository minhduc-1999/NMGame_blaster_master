#pragma once
#include "DynamicGameObject.h"

#define MINE_GRAVITY	0.02f

#define MINE_STATE_ONGROUND	0
#define MINE_STATE_DIE		1

#define MINE_ANI_ONGROUND	0
#define MINE_ANI_DIE		1


class Mine : public CDynamicGameObject
{
public:
	Mine(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};

#pragma once
