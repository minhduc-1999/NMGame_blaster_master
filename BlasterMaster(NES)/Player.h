#pragma once

#include "DynamicGameObject.h"

#define PLAYER_HEIGHT_HIGH	0
#define PLAYER_HEIGHT_LOW	2

#define PLAYER_RUN_SPEED						0.05f
#define PLAYER_JUMP_SPEED_Y						0.29f
#define PLAYER_GRAVITY							0.02f

//define STATE
	//SOPHIA
	#define SOPHIA_STATE_IDLE_RIGHT		000
	#define SOPHIA_STATE_IDLE_LEFT		001
	#define SOPHIA_STATE_RUN_RIGHT		003
	#define SOPHIA_STATE_RUN_LEFT		004
	#define SOPHIA_STATE_JUMP_RIGHT		005
	#define SOPHIA_STATE_JUMP_LEFT		006
	#define SOPHIA_STATE_TURN_RUN		007

//define ANI
	//SOPHIA
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

class Player : public CDynamicGameObject
{
	int height = PLAYER_HEIGHT_HIGH;
	bool isUp = false;
	bool isJumping = false;
public:
	Player(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	bool GetIsUp() { return isUp; };
	void SetIsUp(bool _isUp) { isUp = _isUp; };

	bool GetIsJumping() { return isJumping; };
	void SetIsJumping(bool _isJumping) { isJumping = _isJumping; };
};