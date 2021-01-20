#pragma once
#include "DynamicGameObject.h"

#define JUMPER2_WALKING_SPEED		0.04f
#define JUMPER2_JUMPING_SPEED       0.2f
#define JUMPER2_GRAVITY             0.01f
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
private:
    DWORD startTime;
    int countJump;
    bool canJump;
public:
    Jumper2(float x, float y);
    void SetState(int state);
    int Update(float xMain, float yMain, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    bool IsJumping();
    void Render();
    void SetIsDestroyed() { SetState(JUMPER2_STATE_DIE); }
};

