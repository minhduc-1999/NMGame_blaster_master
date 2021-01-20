#include "DynamicGameObject.h"
#include "BaseBullet.h"

#define MINE_GRAVITY	0.02f

#define MINE_STATE_ONGROUND	0
#define MINE_STATE_DIE		1

#define MINE_ANI_ONGROUND	0
#define MINE_ANI_DIE		1


class Mine : public CDynamicGameObject
{
public:
	Mine(float x, float y);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	vector<LPDYNAMICOBJECT> Fire();
	void SetIsDestroyed() { SetState(MINE_STATE_DIE); }
};
