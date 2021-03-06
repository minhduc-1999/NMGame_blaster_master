#pragma once
#include "DynamicGameObject.h"
class MainPlayer : public CDynamicGameObject
{
protected:
	DWORD lastShot;
	bool isCollisionWithEnemy;
	int alpha;
	bool isJumping;
	bool canShoot;
	int lives;
	bool WinnedBoss;
public:
	MainPlayer(float x, float y) : CDynamicGameObject(x, y) 
	{
		isCollisionWithEnemy = false;
		alpha = 255;
		canShoot = true;
		isJumping = false;
		lastShot = GetTickCount64();
		lives = 3;
		SetTeam(0);
		WinnedBoss = false;
	}
	bool GetWinnedBoss() { return WinnedBoss; }
	void SetWinnedBoss(bool bl) { WinnedBoss = bl; }
	void SetLive(int l) { lives = l; }
	int GetLive() { return lives; }
	bool GetIsJumping() { return isJumping; };
	void SetIsJumping(bool _isJumping) { isJumping = _isJumping; };
	int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	void Render() = 0;
	void SetState(int state) = 0;
	void KeyState(BYTE* states) = 0;
	void OnKeyDown(int KeyCode) = 0;
	void OnKeyUp(int KeyCode) = 0;
};