#pragma once
#include "GameObject.h"
#include "Sound.h"
class CDynamicGameObject :
	public CGameObject
{
protected:
	float dx, dy;
	int nx, ny;
	int state, HP, HPMAX;
	bool isUpdated, isRendered;
	DWORD dt;
	LPANIMATION_SET animation_set;
	bool isShooting;
	bool isDestroyed;
	DWORD TouchTime;
	bool CanTouch;
public:
	CDynamicGameObject(float x, float y);

	int GetNX() { return nx; };
	int GetNY() { return ny; };

	int GetHP() { return HP; };
	void SetHPMAX(int hp) { HP = hp; HPMAX = hp; };
	void SetHP(int hp) { HP = hp; }
	void HPDown(int dmg);
	virtual void SetState(int state);
	int GetState();

	virtual void Render() = 0;

	//if return 0: update successfully
	//if return -1: update fail because last update isn't rendered
	//if return 1: switch scene

	virtual int Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual int Update(float xMain, float yMain,DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	void SetAnimationSet(int ani_set);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	LPCOLLISIONEVENT SweptCollistion(LPGAMEOBJECT coO);
	void CalcNowCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& min_tbx,
		float& min_tby,
		float& nbx,
		float& nby);

	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode) {};
	virtual void OnKeyUp(int KeyCode) {};
	virtual void ResetShooting();
	virtual bool GetIsShooting();
	virtual vector<CDynamicGameObject*> Fire();
	virtual vector<CDynamicGameObject*> Fire(float xMain, float yMain);
	virtual bool GetIsDestroyed();
	virtual void SetIsDestroyed() {}
};
typedef CDynamicGameObject* LPDYNAMICOBJECT;

