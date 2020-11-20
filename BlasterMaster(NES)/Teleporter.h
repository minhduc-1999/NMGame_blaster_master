#ifndef _TELE_H
#define _TELE_H
#include "DynamicGameObject.h"

#define TELEPORTER_TELE_RANGE		10
#define TELEPORTER_DELAY_TIME		15
#define TELEPORTER_SWAP				5

#define TELEPORTER_STATE_GRAY		0
#define TELEPORTER_STATE_GREEN		1

#define TELEPORTER_ANI_GREEN		0
#define TELEPORTER_ANI_GRAY			1


class Teleporter : public CDynamicGameObject
{
public:
	Teleporter(float x, float y);
	bool isTele = false;
	int swap, delay;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
};
#endif // !_TELE_H
