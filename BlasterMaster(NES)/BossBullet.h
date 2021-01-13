#pragma once
#include "BaseBullet.h"

#define BOSS_BULLET_STATE_DESTROY	0
#define BOSS_BULLET_STATE_FIRING	1

#define BOSS_BULLET_ANISET			25
#define BOSS_BULLET_ANI_FIRING		0

#define BOSS_BULLET_DAMAGE	1

class BossBullet :
	public BaseBullet
{
public:
	BossBullet(float x, float y, int team, int nx = 0, int ny = 0);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetSpeed(float spX, float spY);
};