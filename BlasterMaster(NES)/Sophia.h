#ifndef _SOPHIA_H
#define _SOPHIA_H

#include "DynamicGameObject.h"

#define SOPHIA_WIDTH 26
#define SOPHIA_HEIGHT 18

#define SOPHIA_HEIGHT_HIGH	0
#define SOPHIA_HEIGHT_LOW	3

#define SOPHIA_RUN_SPEED						0.05f
#define SOPHIA_JUMP_SPEED_Y						0.29f
#define SOPHIA_GRAVITY							0.02f

//define STATE
#define SOPHIA_STATE_IDLE_RIGHT			000
#define SOPHIA_STATE_IDLE_LEFT			001
#define SOPHIA_STATE_RUN_RIGHT			003
#define SOPHIA_STATE_RUN_LEFT			004
#define SOPHIA_STATE_JUMP_RIGHT			005
#define SOPHIA_STATE_JUMP_LEFT			006
#define SOPHIA_STATE_TURN_RUN			007
#define SOPHIA_STATE_TRANSFORM			8
#define SOPHIA_STATE_FIRING_LEFT		9
#define SOPHIA_STATE_FIRING_RIGHT		10
#define SOPHIA_STATE_FIRING_UP_LEFT		11
#define SOPHIA_STATE_FIRING_UP_RIGHT	12

//define ANI
#define SOPHIA_ANI_RUN_HIGH			00
#define SOPHIA_ANI_RUN_LOW			01
#define SOPHIA_ANI_JUMP_UP			02
#define SOPHIA_ANI_JUMP_DOWN		03
#define SOPHIA_ANI_TURN_RUN			04
#define SOPHIA_ANI_TURN_JUMP		05
#define SOPHIA_ANI_UP				06
#define SOPHIA_ANI_UP_RUN_HIGH		07
#define SOPHIA_ANI_UP_RUN_LOW		8
#define SOPHIA_ANI_UP_JUMP			9
#define SOPHIA_ANI_DOWN				10
#define SOPHIA_ANI_TRANSFORM		11

class Sophia : public CDynamicGameObject
{
	int heightLevel = SOPHIA_HEIGHT_HIGH;
	bool isUp = false;
	bool isJumping = false;
	int alpha = 255;
	bool isCollisionWithEnemy = false;
public:
	Sophia(float x, float y);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	bool GetIsUp() { return isUp; };
	void SetIsUp(bool _isUp) { isUp = _isUp; };

	bool GetIsJumping() { return isJumping; };
	void SetIsJumping(bool _isJumping) { isJumping = _isJumping; };

	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
};

#endif