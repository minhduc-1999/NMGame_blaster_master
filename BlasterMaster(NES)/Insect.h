#pragma once
#include "DynamicGameObject.h"

#define INSECT_FLYING_SPEED		0.02f

#define INSECT_STATE_ALIVE      1
#define INSECT_STATE_DIE        2

#define INSECT_ANI_FLYING	0
#define INSECT_ANI_DIE  	1

class Insect :
    public CDynamicGameObject
{
private:
    float startY;
public:
    Insect(float x, float y);
    void SetState(int state);
    int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    void SetIsDestroyed() { SetState(INSECT_STATE_DIE); }
};

