#pragma once
#include "GameObject.h"
class CDynamicGameObject :
	public CGameObject
{
protected:
	float dx, dy;
	int nx, ny;
	int state;
	bool isUpdated, isRendered;
	DWORD dt;
	LPANIMATION_SET animation_set;
public:
	CDynamicGameObject(float x, float y);

	int GetNX() { return nx; };
	int GetNY() { return ny; };

	virtual void SetState(int state);
	int GetState();

	virtual void Render() = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

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

	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode) {};
	virtual void OnKeyUp(int KeyCode) {};
};
typedef CDynamicGameObject* LPDYNAMICOBJECT;

