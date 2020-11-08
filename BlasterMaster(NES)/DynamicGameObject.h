#pragma once
#include "GameObject.h"
class CDynamicGameObject :
    public CGameObject
{
protected:
	float vx, vy;
	float dx, dy;
	int nx;
	int state;

	DWORD dt;
	vector<LPANIMATION> animations;
public:
	CDynamicGameObject(float x, float y);
	
	virtual void SetState(int state);
	int GetState();

	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void AddAnimation(int aniId);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny);
};
typedef CDynamicGameObject* LPDYNAMICOBJECT;

