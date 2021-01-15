#pragma once
#include "BaseBullet.h"

#define MINIJASON_BULLET_STATE_DESTROY		0
#define MINIJASON_BULLET_STATE_LEFT			1
#define MINIJASON_BULLET_STATE_RIGHT		2

#define MINIJASON_BULLET_ANISET			23
#define MINIJASON_BULLET_ANI_FIRING		0
#define MINIJASON_BULLET_ANI_DESTROY	1

#define MINIJASON_BULLET_SPEED		0.5f
#define MINIJASON_BULLET_DAMAGE		1

class MiniJasonBullet :
	public BaseBullet
{
public:
	MiniJasonBullet(float x, float y, int team, int nx, int ny = 0);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetIsDestroyed() { SetState(MINIJASON_BULLET_STATE_DESTROY); }
};