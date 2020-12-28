#ifndef _BULLET_H
#define _BULLET_H
#include "DynamicGameObject.h"

#define BULLET_FIRING		0
#define BULLET_DESTROY		1

#define SOPHIA_BULLET_HORIZONTAL	0
#define SOPHIA_BULLET_VERTICAL		1
#define CANNON_BULLET_HORIZONTAL	131
#define CANNON_BULLET_VERTICAL		132

class Bullet :
	public CDynamicGameObject
{
	bool isDestroyed;
	DWORD startFiringTime;
public:
	Bullet(float x, float y, int type, int n);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual bool GetIsDestroyed() { return isDestroyed; }
	virtual DWORD GetStartFiringTime() { return startFiringTime; }
};
#endif
