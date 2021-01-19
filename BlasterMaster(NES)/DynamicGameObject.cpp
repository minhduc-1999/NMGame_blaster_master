#include "DynamicGameObject.h"
#include <algorithm>
#include "CGate.h"
#include "Utils.h"

CDynamicGameObject::CDynamicGameObject(float x, float y) :CGameObject(x, y)
{
	nx = 1;
	ny = 0;
	SetSize(16, 16);
	isUpdated = false;
	isRendered = false;
	isShooting = false;
	isDestroyed = false;
	CanTouch = true;
	TouchTime = 0;
}


void CDynamicGameObject::SetState(int state)
{
	this->state = state;
}

int CDynamicGameObject::GetState()
{
	return this->state;
}

void CDynamicGameObject::ResetShooting()
{
	isShooting = !isShooting;
}

bool CDynamicGameObject::GetIsShooting()
{
	return this->isShooting;
}

bool CDynamicGameObject::GetIsDestroyed()
{
	return this->isDestroyed;
}

int CDynamicGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
	return 0;
}

int CDynamicGameObject::Update(float xMain, float yMain,DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
	return 0;
}

void CDynamicGameObject::SetAnimationSet(int ani_set)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani = animation_sets->Get(ani_set);
	animation_set = ani;
}

vector<CDynamicGameObject*> CDynamicGameObject::Fire()
{
	vector<CDynamicGameObject*> temp;
	return temp;
}

vector<CDynamicGameObject*> CDynamicGameObject::Fire(float xMain, float yMain)
{
	vector<CDynamicGameObject*> temp;
	return temp;
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
	D3DXVECTOR2 sSpeed = coO->GetSpeed();
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

LPCOLLISIONEVENT CDynamicGameObject::SweptCollistion(LPGAMEOBJECT coO)
{
	if (this == coO)
		return NULL;
	CCollisionEvent* e = NULL;
	if (CheckIfBound(this->GetBound(), coO->GetBound()))
	{
		e = new CCollisionEvent(0, 0, 0, coO);
	}
	return e;
}

void CDynamicGameObject::CalcNowCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	if (coObjects == NULL)
		return;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptCollistion(coObjects->at(i));
		if (e == NULL) continue;
		if (e->t == 0 && e->nx == 0 && e->ny == 0)
			coEvents.push_back(e);
		else
			delete e;
	}
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
	if (coObjects == NULL)
		return;
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

void CDynamicGameObject::HPDown(int dmg)
{
	HP -= dmg;
	if (HP > HPMAX)
	{
		HP += dmg;
	}
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
		CGate* gate = dynamic_cast<CGate*>(c->obj);

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

void CDynamicGameObject::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& min_tbx, float& min_tby, float& nbx, float& nby)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	min_tbx = 1.0f;
	min_tby = 1.0f;
	int min_ibx = -1;
	int min_iby = -1;

	nbx = 0.0f;
	nby = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];
		if (c->obj->GetType() == 15 || c->obj->GetType() == 60 || c->obj->GetType() == 117)
		{
			if (c->t < min_tby && c->ny != 0) {
				min_tby = c->t; nby = c->ny; min_iby = i;
			}
			if (c->t < min_tbx && c->nx != 0) {
				if (GetType() == 4 && (c->obj->GetPosition().y - 8 > y))
				{
					break;
				}
				else
				{
					min_tbx = c->t; nbx = c->nx; min_ibx = i;
				}
			}

		}
		else if (GetType() == 2 && c->obj->GetType() == 18 && c->obj->GetPosition().y == 168 && c->ny == -1)
		{
			min_tby = c->t; nby = c->ny; min_iby = i;
		}
		else if (c->obj->GetType() == 17 && !(GetType() == 1 || GetType() == 2 || GetType() == 3))
		{
			if (c->t < min_tby && c->ny != 0) {
				min_tby = c->t; nby = c->ny; min_iby = i;
			}
			if (c->t < min_tbx && c->nx != 0) {
				min_tbx = c->t; nbx = c->nx; min_ibx = i;
			}
		}
		else
		{
			if (c->t < min_tx && c->nx != 0) {
				min_tx = c->t; nx = c->nx; min_ix = i;
			}

			if (c->t < min_ty && c->ny != 0) {
				min_ty = c->t; ny = c->ny; min_iy = i;
			}
		}
		
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
