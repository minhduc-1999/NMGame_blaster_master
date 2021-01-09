#include "DynamicGameObject.h"

#define DOME_WIDTH		    18
#define DOME_HEIGHT			17

#define DOME_VERTICAL_WIDTH		        17
#define DOME_VERTICAL_HEIGHT			18

#define DOME_WALKING_SPEED		0.03f
#define DOME_GRAVITY			0.02f

#define DOME_STATE_WALKING_RIGHT		0
#define DOME_STATE_WALKING_LEFT		    100
#define DOME_STATE_WALKING_UP		    200
#define DOME_STATE_WALKING_DOWN	        300
#define DOME_STATE_DROP_DOWN	        400
#define DOME_STATE_DIE			        500

#define DOME_ANI_WALKING_LEFT_RIGHT_UP		0
#define DOME_ANI_WALKING_LEFT_RIGHT_DOWN	1
#define DOME_ANI_WALKING_UP_DOWN_LEFT		2
#define DOME_ANI_WALKING_UP_DOWN_RIGHT		3
#define DOME_ANI_DIE			            4

class Dome : public CDynamicGameObject
{
    int ny;
public:
    Dome(float x, float y);
    int GetNY() { return ny; }
    void SetState(int state);
    int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
};
