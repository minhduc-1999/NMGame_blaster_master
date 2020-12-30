#ifndef _EYEBALL_H
#define _EYEBALL_H

#include "DynamicGameObject.h"
#include "Bullet.h"

#define EYEBALL_STATE_ALIVE	0
#define EYEBALL_STATE_DIE		1

#define EYEBALL_ANI_ALIVE	0
#define EYEBALL_ANI_DIE		1

class Eyeball :
	public CDynamicGameObject
{
private:
	DWORD startTime;
	vector<Bullet*> eyeballBulls;
public:
	~Eyeball()
	{
		for (Bullet* p : eyeballBulls)
			delete p;
		eyeballBulls.clear();
	}
	float startX;
	float startY;
	Eyeball(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, float xMain, float yMain);
	virtual void Render();
	virtual void SetState(int state);
};

#endif // !_EYEBALL_H