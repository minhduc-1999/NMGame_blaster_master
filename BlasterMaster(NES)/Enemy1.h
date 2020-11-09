#pragma once
#include "DynamicGameObject.h"

#define SPEED		0.1
#define GO_LEFT		100
#define GO_RIGHT	200
#define GO_TOP		300
#define GO_BOTTOM	400
#define IDLE		500


class Enemy1 : public CDynamicGameObject
{
public:
	Enemy1(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
};