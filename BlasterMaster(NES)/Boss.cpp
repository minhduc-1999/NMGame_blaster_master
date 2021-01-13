#include "Boss.h"
#include "BossBullet.h"

Boss::Boss(float x, float y, int hand_ani_set_id, int arm_ani_set_id) :CDynamicGameObject(x, y)
{
	SetSize(BOSS_WIDTH, BOSS_HEIGHT);
	startTime = GetTickCount64();
	countBullet = 0;
	SetType(40);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(hand_ani_set_id);
	LPANIMATION_SET ani_set2 = animation_sets->Get(arm_ani_set_id);
	handLeft5 = new Hand(x, y, 5,true);
	handLeft4 = new Hand(x, y, 4, true);
	handLeft3 = new Hand(x, y, 3, true);
	handLeft2 = new Hand(x, y, 2,true);
	handLeft1 = new Hand(x, y, 1,true);
	handLeft5->SetAnimationSet(ani_set);
	handLeft1->SetAnimationSet(ani_set2);
	handLeft2->SetAnimationSet(ani_set2);
	handLeft3->SetAnimationSet(ani_set2);
	handLeft4->SetAnimationSet(ani_set2);
	handLeft5->SetType(41);
	handLeft4->SetType(41);
	handLeft3->SetType(41);
	handLeft2->SetType(41);
	handLeft1->SetType(41);
	handRight5 = new Hand(x, y, 5, false);
	handRight4 = new Hand(x, y, 4, false);
	handRight3 = new Hand(x, y, 3, false);
	handRight2 = new Hand(x, y, 2, false);
	handRight1 = new Hand(x, y, 1, false);
	handRight5->SetAnimationSet(ani_set);
	handRight1->SetAnimationSet(ani_set2);
	handRight2->SetAnimationSet(ani_set2);
	handRight3->SetAnimationSet(ani_set2);
	handRight4->SetAnimationSet(ani_set2);
	handRight5->SetType(41);
	handRight4->SetType(41);
	handRight3->SetType(41);
	handRight2->SetType(41);
	handRight1->SetType(41);
	nx = -1;
	vy = 0;
	vx = 0;

	detroyTime = 0;
}

int Boss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BOSS_STATE_DIE)
	{
		isUpdated = true;
		isRendered = false;
		vector<LPDYNAMICOBJECT> hands = CreateHands();
		for (int i = 0; i < hands.size(); i++)
		{
			hands[i]->SetState(HAND_STATE_DIE);
		}

		handLeft5->Update(dt, coObjects, GetPosition(), 0);
		int temp = handLeft5->GetNextPointIndex();
		handLeft1->Update(dt, coObjects, GetPosition(), temp);
		handLeft2->Update(dt, coObjects, GetPosition(), temp);
		handLeft3->Update(dt, coObjects, GetPosition(), temp);
		handLeft4->Update(dt, coObjects, GetPosition(), temp);

		handRight5->Update(dt, coObjects, GetPosition(), 0);
		int temp2 = handRight5->GetNextPointIndex();
		handRight1->Update(dt, coObjects, GetPosition(), temp2);
		handRight2->Update(dt, coObjects, GetPosition(), temp2);
		handRight3->Update(dt, coObjects, GetPosition(), temp2);
		handRight4->Update(dt, coObjects, GetPosition(), temp2);

		return 0;
	}

	if (isUpdated)
		return -1;
	CDynamicGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	startTime += dt;

	if (startTime > 700 && countBullet < 4)
	{
		isShooting = true;
		startTime = 600;
	}
	else if (countBullet == 4)
	{
		startTime = 0;
		countBullet = 0;
	}

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
			int objType = e->obj->GetType();
			if (objType != 15 && objType != 17)
			{
				if (e->nx != 0)
				{
					x += (1 - e->t) * dx - e->nx * 0.4f;
				}
				else
				{
					y += (1 - e->t) * dy - e->ny * 0.4f;
				}
			}
			switch (objType)
			{
			case 15: case 17:
				if (e->nx != 0) 
				{
					vx = -vx;
					nx = -nx;
				}
				if (e->ny != 0)
				{
					vy = -vy;
				}
				break;
			default:
				break;
			}
		}

		//TODO: Collision logic with dynamic object (bots)
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	handLeft5->Update(dt, coObjects, GetPosition(), 0);
	int temp = handLeft5->GetNextPointIndex();
	handLeft1->Update(dt, coObjects, GetPosition(), temp);
	handLeft2->Update(dt, coObjects, GetPosition(), temp);
	handLeft3->Update(dt, coObjects, GetPosition(), temp);
	handLeft4->Update(dt, coObjects, GetPosition(), temp);

	handRight5->Update(dt, coObjects, GetPosition(), 0);
	int temp2 = handRight5->GetNextPointIndex();
	handRight1->Update(dt, coObjects, GetPosition(), temp2);
	handRight2->Update(dt, coObjects, GetPosition(), temp2);
	handRight3->Update(dt, coObjects, GetPosition(), temp2);
	handRight4->Update(dt, coObjects, GetPosition(), temp2);


	isUpdated = true;
	isRendered = false;
	return 0;
}

void Boss::Render()
{	
	if (isRendered)
		return;
	int ani = BOSS_ANI_FLYING;
	if (state == BOSS_STATE_DIE)
	{
		animation_set->at(ani)->RenderFrame(0,x, y, nx, 253);
		if (GetTickCount64() - detroyTime >= 5000)
		{
			isDestroyed = true;
		}
	}
	else
	{
		animation_set->at(ani)->Render(x, y, nx);
	}

	handLeft1->Render();
	handLeft2->Render();
	handLeft3->Render();
	handLeft4->Render();
	handLeft5->Render();

	handRight1->Render();
	handRight2->Render();
	handRight3->Render();
	handRight4->Render();
	handRight5->Render();

	isRendered = true;
	isUpdated = false;
}

vector<LPDYNAMICOBJECT> Boss::CreateHands()
{
	vector<LPDYNAMICOBJECT> hands;
	
	hands.push_back(handRight1);
	hands.push_back(handRight2);
	hands.push_back(handRight3);
	hands.push_back(handRight4);
	
	hands.push_back(handLeft1);
	hands.push_back(handLeft2);
	hands.push_back(handLeft3);
	hands.push_back(handLeft4);

	hands.push_back(handRight5);
	hands.push_back(handLeft5);
	return hands;
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
	case BOSS_STATE_DIE:
		vy = 0;
		vx = 0;
		detroyTime = GetTickCount64();
		break;
	}
}

vector<LPDYNAMICOBJECT> Boss::Fire(float xMain, float yMain)
{
	vector<LPDYNAMICOBJECT> bossBulls;

	if (countBullet <= 4)
	{
		BossBullet* bullet = new BossBullet(x, y, 1);
		float a = xMain - x;
		float b = yMain - y;
		bullet->SetSpeed(a / sqrt(pow(a, 2) + pow(b, 2)) / 4, b / sqrt(pow(a, 2) + pow(b, 2)) / 4);
		bossBulls.push_back(bullet);

		isShooting = false;
		countBullet++;
	}

	return bossBulls;
}