#ifndef _SKULL_H
#define _SKULL_H
#include "DynamicGameObject.h"
#include "BaseBullet.h"


#define SKULL_FLYING_SPEED			0.12f

#define SKULL_STATE_FLYING_LEFT		0
#define SKULL_STATE_FLYING_RIGHT	1
#define SKULL_STATE_FIRE_LEFT		2
#define SKULL_STATE_FIRE_RIGHT		3
#define SKULL_STATE_DIE				4

#define SKULL_ANI_FLYING			0
#define SKULL_ANI_FIRE				1
#define SKULL_ANI_DIE				2



class Skull : public CDynamicGameObject
{
private:
	float startX;
	float startY;
	DWORD startTime;
	bool shooted;
public:
	Skull(float x, float y);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	vector<LPDYNAMICOBJECT> Fire();
	void SetIsDestroyed() { SetState(SKULL_STATE_DIE); }
};
#endif // !_SKULL_H

