#pragma once
#include "BaseBullet.h"

#define JASON_BULLET_STATE_DETROY	-1
#define JASON_BULLET_STATE_FLY_LEFT	0
#define JASON_BULLET_STATE_FLY_RIGHT	1
#define JASON_BULLET_STATE_FLY_TOP	2
#define JASON_BULLET_STATE_FLY_BOTTOM	3

#define JASON_BULLET_ANI_DETROY	0
#define JASON_BULLET_ANI_FLY_LEFT	1

#define JASON_BULLET_SPEED	0.5f
#define JASON_BULLET_DAMAGE	2
#define JASON_BULLET_ALIVE_TIME	200
#define JASON_BULLET_ANISET	20
class JasonBullet : public BaseBullet
{
public:
	JasonBullet(float x, float y, int team, int nx, int ny = 0);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
};