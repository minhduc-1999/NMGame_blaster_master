#include "Boss.h"

Boss::Boss(float x, float y, int hand_ani_set_id, int arm_ani_set_id) :CDynamicGameObject(x, y)
{
	SetSize(BOSS_WIDTH, BOSS_HEIGHT);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(hand_ani_set_id);
	LPANIMATION_SET ani_set2 = animation_sets->Get(arm_ani_set_id);
	handLeft5 = new Hand(x, y, 5,true);
	handLeft1 = new Hand(x, y, 4, true);
	handLeft2 = new Hand(x, y, 3, true);
	handLeft3 = new Hand(x, y, 2,true);
	handLeft4 = new Hand(x, y, 1,true);
	handLeft5->SetAnimationSet(ani_set);
	handLeft1->SetAnimationSet(ani_set2);
	handLeft2->SetAnimationSet(ani_set2);
	handLeft3->SetAnimationSet(ani_set2);
	handLeft4->SetAnimationSet(ani_set2);
	handLeft5->SetState(HAND_STATE_FLYING_DOWN);
	handLeft2->SetState(HAND_STATE_FLYING_DOWN);
	handLeft3->SetState(HAND_STATE_FLYING_DOWN);
	handLeft4->SetState(HAND_STATE_FLYING_DOWN);
	handLeft1->SetState(HAND_STATE_FLYING_DOWN);
	handRight5 = new Hand(x, y, 5, false);
	handRight1 = new Hand(x, y, 4, false);
	handRight2 = new Hand(x, y, 3, false);
	handRight3 = new Hand(x, y, 2, false);
	handRight4 = new Hand(x, y, 1, false);
	handRight5->SetAnimationSet(ani_set);
	handRight1->SetAnimationSet(ani_set2);
	handRight2->SetAnimationSet(ani_set2);
	handRight3->SetAnimationSet(ani_set2);
	handRight4->SetAnimationSet(ani_set2);
	handRight5->SetState(HAND_STATE_FLYING_DOWN);
	handRight2->SetState(HAND_STATE_FLYING_DOWN);
	handRight3->SetState(HAND_STATE_FLYING_DOWN);
	handRight4->SetState(HAND_STATE_FLYING_DOWN);
	handRight1->SetState(HAND_STATE_FLYING_DOWN);
	//handRight = new Hand(x + 20, y, false, arm_ani_set_id);
	//handRight->SetAnimationSet(ani_set);
	//handRight->SetState(HAND_STATE_FLYING);
	nx = -1;
	vy = 0;
	vx = 0;
	lastTime = GetTickCount();
}

void Boss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpdated)
		return;
	DWORD now = GetTickCount();
	if (now - lastTime >= 3000)
	{
		handLeft5->SetState(HAND_STATE_FLYING_BIAS);
		handRight5->SetState(HAND_STATE_FLYING_BIAS);
		lastTime = now;
	}
	CDynamicGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, ntx, nty;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, ntx, nty);

		// block 
		x += min_tx * dx + ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + nty * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			// if e->obj is Gate 
			if (dynamic_cast<CDynamicGameObject*>(e->obj))
			{
				CDynamicGameObject* obj = dynamic_cast<CDynamicGameObject*>(e->obj);
				if (this->team == obj->GetTeam())
				{
					x += (1 - min_tx) * dx - ntx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
					y += (1 - min_ty) * dy - nty * 0.4f;
					for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
					return;
				}
			}
		}

		if (ntx != 0)
		{
			vx = -vx;
			nx = -nx;
		}

		if (nty != 0)
		{
			vy = -vy;
		}

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	handLeft5->Update(dt, coObjects, GetPosition());
	handLeft1->SetState(handLeft5->GetState());
	handLeft2->SetState(handLeft5->GetState());
	handLeft3->SetState(handLeft5->GetState());
	handLeft4->SetState(handLeft5->GetState());
	handLeft1->Update(dt, coObjects, GetPosition());
	handLeft2->Update(dt, coObjects, GetPosition());
	handLeft3->Update(dt, coObjects, GetPosition());
	handLeft4->Update(dt, coObjects, GetPosition());

	handRight5->Update(dt, coObjects, GetPosition());
	handRight1->SetState(handRight5->GetState());
	handRight2->SetState(handRight5->GetState());
	handRight3->SetState(handRight5->GetState());
	handRight4->SetState(handRight5->GetState());
	handRight1->Update(dt, coObjects, GetPosition());
	handRight2->Update(dt, coObjects, GetPosition());
	handRight3->Update(dt, coObjects, GetPosition());
	handRight4->Update(dt, coObjects, GetPosition());
	//handRight->Update(dt, coObjects, GetPosition());


	isUpdated = true;
	isRendered = false;
}

void Boss::Render()
{	
	if (isRendered)
		return;
	int ani = BOSS_ANI_FLYING;

	animation_set->at(ani)->Render(x, y, nx);

	handLeft4->Render();
	handLeft3->Render();
	handLeft2->Render();
	handLeft1->Render();
	handLeft5->Render();

	handRight4->Render();
	handRight3->Render();
	handRight2->Render();
	handRight1->Render();
	handRight5->Render();

	isRendered = true;
	isUpdated = false;
}

void Boss::SetState(int state)
{
	CDynamicGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STATE_FLYING:
		vy = BOSS_FLYING_SPEED_Y;
		vx = -BOSS_FLYING_SPEED_X;
		break;
	}
}