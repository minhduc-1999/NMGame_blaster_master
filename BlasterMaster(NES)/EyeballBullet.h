#pragma once
#include "BaseBullet.h"

#define EYEBALL_BULLET_STATE_DESTROY	0
#define EYEBALL_BULLET_STATE_FIRING		1

#define EYEBALL_BULLET_ANISET			21
#define EYEBALL_BULLET_ANI_FIRING		0
#define EYEBALL_BULLET_ANI_DESTROY		1

#define EYEBALL_BULLET_DAMAGE	1

class EyeballBullet :
	public BaseBullet
{
public:
	EyeballBullet(float x, float y, int team, int nx = 0, int ny = 0);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetSpeed(float spX, float spY);
};