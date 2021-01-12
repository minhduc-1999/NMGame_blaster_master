#include "DynamicGameObject.h"

#define HAND_WIDTH		            18
#define HAND_HEIGHT			        32
#define ARM_WIDTH		            16
#define ARM_HEIGHT			        17


#define HAND_FLYING_SPEED_X           0.04f
#define HAND_FLYING_SPEED_Y           0.04f

#define HAND_STATE_FLYING       0
#define HAND_STATE_DIE          1

#define HAND_ANI               0

class Hand : public CDynamicGameObject
{
    D3DXVECTOR2 nextPosition;
    D3DXVECTOR2 currentPosition;
    int currentPointIndex;
    int nextPointIndex;
    D3DXVECTOR2 handPosition, bossPosition;
    int nx;
    int ny;
    int index;
    bool isLeftHand;
    DWORD lastTime = 0;
    DWORD detroyTime = 0;
    float leftSide, topSide, rightSide, bottomSide;
    float CalcD(D3DXVECTOR2 a, D3DXVECTOR2 b);
public:
    Hand(float x, float y,int _index, bool _isLeftHand);
    void SetState(int state);
    int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR2 bossPosition, int nextPointIndex);
    //int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR2 bossPosition,int beforeNodeNextPosIndex);
    void Render();
    int GetNextPointIndex() { return nextPointIndex; }
};