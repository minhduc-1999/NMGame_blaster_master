#pragma once
#include "GameObject.h"


#define MINE_STATE_ONGROUND	0
#define MINE_STATE_DIE		1

#define MINE_ANI_ONGROUND	0
#define MINE_ANI_DIE		1


class Mine : public GameObject
{
	virtual void Update(DWORD dt);
	virtual void Render();
public:
	virtual void SetState(int state);
};

