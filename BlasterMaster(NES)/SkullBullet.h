#pragma once
#include "BaseBullet.h"

#define SKULL_BULLET_STATE_DESTROY	0
#define SKULL_BULLET_STATE_FIRING	1
#define SKULL_BULLET_STATE_ONGROUND	2

#define SKULL_BULLET_ANISET			22
#define SKULL_BULLET_ANI_FIRING		0
#define SKULL_BULLET_ANI_DESTROY	1

#define SKULL_BULLET_DAMAGE	1

class SkullBullet :
	public BaseBullet
{
public:
	SkullBullet(float x, float y, int team, int nx = 0, int ny = 0);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
};