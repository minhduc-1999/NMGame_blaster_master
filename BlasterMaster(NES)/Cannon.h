#ifndef _CANNON_H
#define _CANNON_H
#include "DynamicGameObject.h"
#include "Bullet.h"

#define CANNON_STATE_ALIVE		0
#define CANNON_STATE_DIE		1
#define CANNON_STATE_FIRE_HOR	2
#define CANNON_STATE_FIRE_VER	3

#define CANNON_ANI_ALIVE	0
#define CANNON_ANI_DIE		1

class Cannon :
	public CDynamicGameObject
{
private:
	DWORD startTime;
	vector<Bullet*> cannonBulls;
	bool hor;
public:
	Cannon(float x, float y);
	~Cannon() 
	{
		for (Bullet* p : cannonBulls)
			delete p;
		cannonBulls.clear();
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};
#endif
