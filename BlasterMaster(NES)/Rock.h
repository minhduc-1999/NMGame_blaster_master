#pragma once
#include "DynamicGameObject.h"

#define ROCK_STATE_ONMAP	0
#define ROCK_STATE_DESTROY	1

#define ROCK_ANISET			60
#define ROCK_ANI_ONMAP		0
#define ROCK_ANI_DESTROY	1

class Rock
	: public CDynamicGameObject
{
public:
	Rock(float x, float y);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetIsDestroyed() { SetState(ROCK_STATE_DESTROY); }
};
