#ifndef _ORB_H
#define _ORB_H
#include "DynamicGameObject.h"

#define ORB_RUNNING_SPEED		0.08f

#define ORB_STATE_IDLE			0
#define ORB_STATE_ROLLING_LEFT	100
#define ORB_STATE_ROLLING_RIGHT	200
#define ORB_STATE_RUNNING_LEFT  300
#define ORB_STATE_RUNNING_RIGHT 400
#define ORB_STATE_DIE           500

#define HUNTING_RANGE           40

#define ORB_ANI_ROLL             0
#define ORB_ANI_DIE             1

class Orb :
    public CDynamicGameObject
{
public:
    Orb(float x, float y);
    void SetState(int state);
    int Update(float xMain, float yMain, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    void SetIsDestroyed() { SetState(ORB_STATE_DIE); }
};
#endif // !_ORB_H



