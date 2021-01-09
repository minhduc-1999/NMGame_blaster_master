#pragma once
#include "DynamicGameObject.h"

class BaseBullet : public  CDynamicGameObject
{
protected:
	DWORD startFiringTime;
public:
	BaseBullet(float x, float y, int team, int nx, int ny = 0);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	void Render() = 0;
	void SetState(int state) = 0;
};