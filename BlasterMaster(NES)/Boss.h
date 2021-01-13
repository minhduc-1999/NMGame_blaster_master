#include "DynamicGameObject.h"
#include "Hand.h"
#include "BaseBullet.h"

#define BOSS_WIDTH		    60
#define BOSS_HEIGHT			65


#define BOSS_FLYING_SPEED_X		0.02f
#define BOSS_FLYING_SPEED_Y		0.01f

#define BOSS_STATE_FLYING   0
#define BOSS_STATE_DIE      1

#define BOSS_ANI_FLYING 0

class Boss : public CDynamicGameObject
{
    Hand* handRight5 , * handRight4, * handRight3, * handRight2, * handRight1;
    Hand* handLeft5, * handLeft4 , * handLeft3, * handLeft2, * handLeft1;
    DWORD detroyTime;
    DWORD startTime;
    int countBullet;
public:
    Boss(float x, float y, int hand_ani_set_id, int arm_ani_set_id);
    void SetState(int state);
    int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    vector<LPDYNAMICOBJECT> Fire(float xMain, float yMain);
    vector<LPDYNAMICOBJECT> CreateHands();
};
