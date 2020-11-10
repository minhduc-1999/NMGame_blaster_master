#ifndef _SKULL_H
#define _SKULL_H
#include "DynamicGameObject.h"


#define SKULL_FLYING_SPEED			0.05f

#define SKULL_STATE_FIRE			1
#define SKULL_STATE_FLYING_RIGHT	100
#define SKULL_STATE_FLYING_LEFT		200

#define SKULL_ANI_FLYING_LEFT		0
#define SKULL_ANI_FIRE				1



class Skull : public CDynamicGameObject
{
public:
	Skull (float x, float y) : CDynamicGameObject(x, y) {}
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};
#endif // !_SKULL_H

