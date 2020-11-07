#ifndef _ORB_H
#define _ORB_H
#include "DynamicGameObject.h"

#define ORB_RUNNING_SPEED		0.05f

#define ORB_STATE_IDLE			0
#define ORB_STATE_RUNNING_LEFT	100
#define ORB_STATE_RUNNING_RIGHT	200
#define ORB_STATE_TURN_LEFT 	300
#define ORB_STATE_TURN_RIGHT	400

#define ORB_ANI_RUN_LEFT		0
#define ORB_ANI_TURN_RIGHT      1

class Orb :
    public CDynamicGameObject
{
public:
    Orb(float x, float y) : CDynamicGameObject(x, y) {}
    void SetState(int state);
    void Update(DWORD dt);
    void Render();
};
#endif // !_ORB_H



