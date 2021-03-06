#ifndef _MINIJASON_H
#define _MINIJASON_H
#include "MainPlayer.h"
#include "BaseBullet.h"

#define MINIJASON_WIDTH			10
#define MINIJASON_HEIGHT		17
#define MINIJASON_DOWN_WIDTH	17
#define MINIJASON_DOWN_HEIGHT	10
#define MINIJASON_HEIGHT_DIE	60

#define MINIJASON_RUN_SPEED						0.04f
#define MINIJASON_DOWN_RUN_SPEED				0.03f
#define MINIJASON_CLIMB_SPEED_Y					0.03f
#define MINIJASON_JUMP_SPEED_Y					0.21f
#define MINIJASON_GRAVITY						0.01f

//define STATE
#define MINIJASON_STATE_IDLE_RIGHT		000
#define MINIJASON_STATE_IDLE_LEFT		001
#define MINIJASON_STATE_RUN_RIGHT		002
#define MINIJASON_STATE_RUN_LEFT		003
#define MINIJASON_STATE_JUMP_RIGHT		004
#define MINIJASON_STATE_JUMP_LEFT		005
#define MINIJASON_STATE_TRANSFORM		006
#define MINIJASON_STATE_CLIMB			007
#define MINIJASON_STATE_DIE				8

//define ANI
#define MINIJASON_ANI_IDLE				00
#define MINIJASON_ANI_RUN				01
#define MINIJASON_ANI_JUMP				02
#define MINIJASON_ANI_DOWN_RUN			03
#define MINIJASON_ANI_CLIMB				04
#define MINIJASON_ANI_DIE				05

class MiniJason : public MainPlayer
{
	bool canGoOvw = false;
	bool isDown = false;
	bool isCollisionWithSophia = false;
	bool canClimb = false;
	float yBeforeDrop;
public:
	MiniJason(float x, float y);
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	bool IsCollisionWithSophia(){ return isCollisionWithSophia; }

	bool GetIsDown() { return isDown; };
	void SetIsDown(bool _isDown) { isDown = _isDown; };

	bool GetIsJumping() { return isJumping; };
	void SetIsJumping(bool _isJumping) { isJumping = _isJumping; };

	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	BaseBullet* Shoot();
	bool CanShoot() { return canShoot; }
};
#endif
