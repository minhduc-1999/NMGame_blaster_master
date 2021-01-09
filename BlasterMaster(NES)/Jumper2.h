#pragma once
#include "DynamicGameObject.h"

#define JUMPER2_WALKING_SPEED		0.02f
#define JUMPER2_JUMP_DELAY          20
#define JUMPER2_GRAVITY             0.02f
#define JUMPER2_STATE_IDLE			0
#define JUMPER2_STATE_WALKING_RIGHT	100
#define JUMPER2_STATE_WALKING_LEFT	200
#define JUMPER2_STATE_JUMPING_RIGHT 300
#define JUMPER2_STATE_JUMPING_LEFT  400
#define JUMPER2_STATE_DIE           500

#define JUMPER2_ANI_WALK    		0
#define JUMPER2_ANI_DIE    		    1

class Jumper2 :
    public CDynamicGameObject
{
public:
    Jumper2(float x, float y);
    void SetState(int state);
    int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    bool IsJumping();
    int jump;
    void Render();
};

