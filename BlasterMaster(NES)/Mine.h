#include "DynamicGameObject.h"

#define MINE_GRAVITY	0.02f

#define MINE_STATE_ONGROUND	0
#define MINE_STATE_DIE		1

#define MINE_ANI_ONGROUND	0
#define MINE_ANI_DIE		1


class Mine : public CDynamicGameObject
{
private:
	DWORD startTime;
public:
	Mine(float x, float y);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	vector<LPDYNAMICOBJECT> Fire();
};
