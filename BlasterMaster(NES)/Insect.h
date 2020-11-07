#pragma once
#include "GameObject.h"

#define INSECT_FLYING_SPEED		0.1f

#define INSECT_STATE_IDLE			0
#define INSECT_STATE_FLYING_RIGHT	100
#define INSECT_STATE_FLYING_LEFT	200

#define INSECT_ANI_FLY_LEFT		0

class Insect :
    public CGameObject
{
public:
    Insect() : CGameObject(){}
    void SetState(int state);
    void Update(DWORD dt);
    void Render();
};

