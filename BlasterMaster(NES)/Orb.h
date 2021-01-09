#ifndef _ORB_H
#define _ORB_H
#include "DynamicGameObject.h"

#define ORB_RUNNING_SPEED		0.05f

#define ORB_STATE_IDLE			0
#define ORB_STATE_ROLLING_LEFT	100
#define ORB_STATE_ROLLING_RIGHT	200
#define ORB_STATE_RUNNING_LEFT  300
#define ORB_STATE_RUNNING_RIGHT 400
#define ORB_STATE_DIE           500


#define ORB_ANI_ROLL             0
#define ORB_ANI_DIE             1

class Orb :
    public CDynamicGameObject
{
public:
    Orb(float x, float y);
    void SetState(int state);
    int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
};
#endif // !_ORB_H



