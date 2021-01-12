#pragma once
#include "BaseBullet.h"

#define CANNON_BULLET_STATE_DESTROY		0
#define CANNON_BULLET_STATE_LEFT		1
#define CANNON_BULLET_STATE_RIGHT		2
#define CANNON_BULLET_STATE_UP			3
#define CANNON_BULLET_STATE_DOWN		4

#define CANNON_BULLET_ANISET			21
#define CANNON_BULLET_ANI_FIRING		0
#define CANNON_BULLET_ANI_DESTROY		1

#define CANNON_BULLET_SPEED		0.4f
#define CANNON_BULLET_DAMAGE	1

class CannonBullet :
	public BaseBullet
{
public:
	CannonBullet(float x, float y, int team, int nx, int ny);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
};