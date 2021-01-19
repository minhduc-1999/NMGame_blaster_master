#pragma once
#include "DynamicGameObject.h"

#define BROKABLE_BRICK_STATE_ONMAP			0
#define	BROKABLE_BRICK_STATE_DESTROY		1

#define BROKABLE_BRICK_ANISET			117
#define	BROKABLE_BRICK_ANI_ONMAP		0
#define BROKABLE_BRICK_ANI_DESTROY		1

class BrokableBrick :
	public CDynamicGameObject
{
public:
	BrokableBrick(float x, float y);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetIsDestroyed() { SetState(BROKABLE_BRICK_STATE_DESTROY); }
};