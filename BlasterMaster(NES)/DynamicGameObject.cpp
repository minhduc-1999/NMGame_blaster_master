#include "DynamicGameObject.h"
#include <algorithm>

CDynamicGameObject::CDynamicGameObject(float x, float y) :CGameObject(x, y)
{
	nx = 1;
	SetSize(16, 16);
}


void CDynamicGameObject::SetState(int state)
{
	this->state = state;
}

int CDynamicGameObject::GetState()
{
	return this->state;
}



void CDynamicGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

void CDynamicGameObject::AddAnimation(int aniId)
{
	LPANIMATION ani = CAnimationManager::GetInstance()->Get(aniId);
	animations.push_back(ani);
}

LPCOLLISIONEVENT CDynamicGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	if (this == coO)
		return NULL;
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	Rect sBound = coO->GetBound();
	sl = sBound.left;
	st = sBound.top;
	sr = sBound.right;
	sb = sBound.bottom;

	//coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	D3DXVECTOR3 sSpeed = coO->GetSpeed();
	svx = sSpeed.x;
	svy = sSpeed.y;

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	//GetBoundingBox(ml, mt, mr, mb);
	Rect mBound = this->GetBound();
	ml = mBound.left;
	mt = mBound.top;
	mr = mBound.right;
	mb = mBound.bottom;

	CGame::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CDynamicGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (e == NULL) continue;
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CDynamicGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}