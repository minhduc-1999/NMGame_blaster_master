#ifndef _EYEBALL_H
#define _EYEBALL_H

#include "DynamicGameObject.h"
#include "BaseBullet.h"

#define EYEBALL_STATE_ALIVE	0
#define EYEBALL_STATE_DIE		1

#define EYEBALL_ANI_ALIVE	0
#define EYEBALL_ANI_DIE		1

class Eyeball :
	public CDynamicGameObject
{
private:
	DWORD startTime;
public:
	float startX;
	float startY;
	Eyeball(float x, float y);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	vector<LPDYNAMICOBJECT> Fire(float xMain, float yMain);
};

#endif // !_EYEBALL_H