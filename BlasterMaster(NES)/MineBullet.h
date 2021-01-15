#pragma once
#include "BaseBullet.h"

#define MINE_BULLET_STATE_DESTROY	0
#define MINE_BULLET_STATE_FIRING	1

#define MINE_BULLET_ANISET			21
#define MINE_BULLET_ANI_FIRING		0
#define MINE_BULLET_ANI_DESTROY		1

#define MINE_BULLET_DAMAGE	1

class MineBullet :
	public BaseBullet
{
private:
	float startY;
	bool isFalling;
public:
	MineBullet(float x, float y, int team, int nx = 0, int ny = 0);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetSpeed(float spX, float spY);
};