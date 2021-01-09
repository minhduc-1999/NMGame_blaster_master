#pragma once
#include "DynamicGameObject.h"
#include "BaseBullet.h"

#define JASON_WIDTH			24
#define JASON_HEIGHT		32

#define JASON_SPEED			0.06f

#define JASON_STATE_IDLE			001
#define JASON_STATE_RUN_RIGHT		002
#define JASON_STATE_RUN_LEFT		003
#define JASON_STATE_RUN_TOP			004
#define JASON_STATE_RUN_BOTTOM		005
#define JASON_STATE_DIE				006

#define JASON_ANI_IDLE_LEFT			00
#define JASON_ANI_IDLE_TOP			01
#define JASON_ANI_IDLE_BOTTOM		02
#define JASON_ANI_RUN_LEFT			03
#define JASON_ANI_RUN_TOP			04
#define JASON_ANI_RUN_BOTTOM		05
#define JASON_ANI_DIE				06


#define JASON_SHOOTING_DELAY	200
#define JASON_UNSTOPABLE	2000
class Jason : public CDynamicGameObject
{
	DWORD lastShot;
	bool canGoArea;
	bool canShoot;
public:
	bool CanShoot() { return canShoot; }
	Jason(float x, float y);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	Rect GetBound();
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	BaseBullet* Shoot();
};