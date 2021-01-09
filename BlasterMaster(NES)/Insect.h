#pragma once
#include "DynamicGameObject.h"

#define INSECT_FLYING_SPEED		0.02f
#define INSECT_JUMPING_TIME       2

#define INSECT_STATE_IDLE			0
#define INSECT_STATE_FLYDOWN_RIGHT	100
#define INSECT_STATE_FLYDOWN_LEFT	200
#define INSECT_STATE_FLYUP_RIGHT	300
#define INSECT_STATE_FLYUP_LEFT 	400
#define INSECT_STATE_DIE         	500

#define INSECT_ANI_FLYING	0
#define INSECT_ANI_DIE  	1

class Insect :
    public CDynamicGameObject
{
public:
    Insect(float x, float y);
    int jump;
    void SetState(int state);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
};

