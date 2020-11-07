#pragma once
#include "DynamicGameObject.h"

#define INSECT_FLYING_SPEED		0.1f

#define INSECT_STATE_IDLE			0
#define INSECT_STATE_FLYING_RIGHT	100
#define INSECT_STATE_FLYING_LEFT	200

#define INSECT_ANI_FLY_LEFT		0

class Insect :
    public CDynamicGameObject
{
public:
    Insect(float x, float y) : CDynamicGameObject(x, y){}
    void SetState(int state);
    void Update(DWORD dt);
    void Render();
};

