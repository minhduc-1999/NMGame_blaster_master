#ifndef _BULLET_H
#define _BULLET_H
#include "DynamicGameObject.h"

#define BULLET_FIRING		0
#define BULLET_DESTROY		1

#define BULLET_HORIZONTAL	0
#define BULLET_VERTICAL		1
class Bullet :
	public CDynamicGameObject
{
	bool isDestroyed;
	DWORD startFiringTime;
public:
	Bullet(float x, float y, int a);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void SetDir(int dir);
	virtual bool GetIsDestroyed() { return isDestroyed; }
	virtual DWORD GetStartFiringTime() { return startFiringTime; }
};
#endif
