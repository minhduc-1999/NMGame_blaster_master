#include "DynamicGameObject.h"

#define HAND_WIDTH		            18
#define HAND_HEIGHT			        32
#define ARM_WIDTH		            16
#define ARM_HEIGHT			        17


#define HAND_FLYING_SPEED_X           0.04f
#define HAND_FLYING_SPEED_Y           0.04f

#define HAND_STATE_FLYING_LEFT           0
#define HAND_STATE_FLYING_UP             1
#define HAND_STATE_FLYING_RIGHT          2
#define HAND_STATE_FLYING_DOWN           3
#define HAND_STATE_FLYING_BIAS           4

#define HAND_ANI               0

class Hand : public CDynamicGameObject
{
    D3DXVECTOR2 handPosition, bossPosition;
    int nx;
    int ny;
    int index;
    bool isLeftHand;
    DWORD lastTime = 0;
    float leftSide, topSide, rightSide, bottomSide;
public:
    Hand(float x, float y,int _index, bool _isLeftHand);
    void SetState(int state);
    int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR2 bossPosition);
    void Render();
};