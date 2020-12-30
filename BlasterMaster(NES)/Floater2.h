#include "DynamicGameObject.h"
#include "Bullet.h"

#define FLOATER2_WIDTH		    18
#define FLOATER2_HEIGHT			15

#define FLOATER2_FLYING_SPEED_X		0.02f
#define FLOATER2_FLYING_SPEED_Y		0.04f
        
#define FLOATER2_STATE_FLYING_RIGHT		    0
#define FLOATER2_STATE_FLYING_LEFT		    100
#define FLOATER2_STATE_DIE			        200
        
#define FLOATER2_ANI_FLYING		    0
#define FLOATER2_ANI_FIRE			1
#define FLOATER2_ANI_DIE			2

class Floater2 : public CDynamicGameObject
{
private:
    vector<Bullet*> floaterBulls;
    DWORD startTime;
public:
    Floater2(float x, float y);
    ~Floater2()
    {
        for (Bullet* p : floaterBulls)
            delete p;
        floaterBulls.clear();
    }
    void SetState(int state);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, float xMain, float yMain);
    void Render();
    int FindMain(float xF, float yF, float xMain, float yMain);
};
