#pragma once
#include "BaseBullet.h"

#define SOPHIA_BULLET_STATE_DESTROY		0
#define SOPHIA_BULLET_STATE_LEFT		1
#define SOPHIA_BULLET_STATE_RIGHT		2
#define SOPHIA_BULLET_STATE_UP			3

#define SOPHIA_BULLET_ANISET			20
#define SOPHIA_BULLET_ROCKET_ANISET		27
#define SOPHIA_BULLET_ANI_HORIZONTAL	0
#define SOPHIA_BULLET_ANI_VERTICAL		1
#define SOPHIA_BULLET_ANI_DESTROY		2

#define SOPHIA_BULLET_SPEED		0.5f
#define SOPHIA_BULLET_DAMAGE	2

class SophiaBullet :
	public BaseBullet
{
	int type;
public:
	SophiaBullet(float x, float y, int team, int t, int nx, int ny = 0);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetSpeed(float spX, float spY);
};