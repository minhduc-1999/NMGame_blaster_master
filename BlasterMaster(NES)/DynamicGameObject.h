#pragma once
#include "GameObject.h"
class CDynamicGameObject :
    public CGameObject
{
protected:
	float vx, vy;
	int nx;
	int state;
	vector<LPANIMATION> animations;
public:
	CDynamicGameObject(float x, float y);
	
	virtual void SetState(int state);
	int GetState();

	void SetSpeed(float vx, float vy);
	D3DXVECTOR3 GetSpeed();
	virtual void Update(DWORD dt);
	void AddAnimation(int aniId);
};
typedef CDynamicGameObject* LPDYNAMICOBJECT;

