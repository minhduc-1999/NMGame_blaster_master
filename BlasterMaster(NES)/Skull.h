#ifndef _SKULL_H
#define _SKULL_H
#include "DynamicGameObject.h"


#define SKULL_FLYING_SPEED			0.08f

#define SKULL_STATE_FLYING_LEFT		0
#define SKULL_STATE_FLYING_RIGHT	1
#define SKULL_STATE_FIRE_LEFT		2
#define SKULL_STATE_FIRE_RIGHT		3

#define SKULL_ANI_FLYING			0
#define SKULL_ANI_FIRE				1



class Skull : public CDynamicGameObject
{
public:
	Skull(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
};
#endif // !_SKULL_H

