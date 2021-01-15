#include "DynamicGameObject.h"

#define WORM_WIDTH		    18
#define WORM_HEIGHT			10

#define WORM_WALKING_SPEED		0.03f
#define WORM_GRAVITY			0.06f
#define WORM_GO_UP_SPEED		0.06f

#define WORM_STATE_WALKING_RIGHT		0
#define WORM_STATE_GO_UP                100
#define WORM_STATE_WALKING_LEFT		    200
#define WORM_STATE_DIE			        300

#define WORM_ANI_WALKING		0
#define WORM_ANI_DIE			1

class Worm : public CDynamicGameObject
{
    bool isCollisionWithMagma = false;
public:
    Worm(float x, float y);
    void SetState(int state);
    int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
};
