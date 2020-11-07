#pragma once
#include "DynamicGameObject.h"

#define JUMPER2_WALKING_SPEED		0.05f

#define JUMPER2_STATE_IDLE			0
#define JUMPER2_STATE_WALKING_RIGHT	100
#define JUMPER2_STATE_WALKING_LEFT	200

#define JUMPER2_ANI_WALK_LEFT		0

class Jumper2 :
    public CDynamicGameObject
{
public:
    Jumper2(float x, float y) : CDynamicGameObject(x, y) {}
    void SetState(int state);
    void Update(DWORD dt);
    void Render();
};

