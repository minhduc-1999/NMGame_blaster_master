#include "SectionBoss.h"
#include "Jason.h"
#include "Textures.h"
#include "BossBullet.h"
#include "Hand.h"
#include "Boss.h"
void SectionBoss::Load(SaveData* data, D3DXVECTOR2 mainPos)
{
	Section::Load();
	if (!((Jason*)mainPlayer)->GetWinnedBoss())
	{
		Sound::getInstance()->stop("lvl2");
		Sound::getInstance()->play("Boss", false, 1);
	}
	startTime = GetTickCount64();
	if (mainPlayer != NULL)
	{
		//D3DXVECTOR2 pos = mainPlayer->GetPosition();
		//DebugOut("[Pos player trans before load]\tx: %f, y: %f\n", pos.x, pos.y);
		DebugOut("[ERROR] main object was created before in ovw!\n");
		return;
	}
}
Boss* _boss;
int SectionBoss::Update(DWORD dt)
{
	if (_boss != NULL)
	{
		if (_boss->GetIsDestroyed())
		{
			period = 2;
			((Jason*)mainPlayer)->SetWinnedBoss(true);
			Sound::getInstance()->play("lvl2", true, 0);
		}
	}
	if (period == 0)
	{
		if (GetTickCount64() - startTime >  7000)
		{
			period = 1;
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(40);
			Boss* boss = new Boss(895, 615, 41, 42);
			_boss = boss;
			boss->SetAnimationSet(ani_set);
			boss->SetState(BOSS_STATE_FLYING);
			grids[0]->AddDynamicObj(boss);
			vector<LPDYNAMICOBJECT> hands;
			hands = boss->CreateHands();
			for (int i = 0; i < hands.size(); i++)
			{
				grids[0]->AddDynamicObj(hands[i]);
			}
			Sound::getInstance()->stop("Boss");
		}
	}
	if (period == 1) {
		vector<LPGAMEOBJECT> coObjs;
		vector<LPGAMEOBJECT>* temp = grids.at(0)->GetcoObjectList();
		coObjs.insert(coObjs.end(), temp->begin(), temp->end());
		grids[0]->Update(dt, &coObjs, mainPlayer->GetPosition().x, mainPlayer->GetPosition().y);
		mainPlayer->Update(dt, &coObjs);
		return 0;
	}
	if (period == 2) {
		vector<LPGAMEOBJECT> coObjs;
		Rect camPos = CGame::GetInstance()->GetCamBound();
		vector<int> camBoundGrid = GetBoundGrid(camPos);
		for (int i = 0; i < camBoundGrid.size(); i++)
		{
			if (grids.find(camBoundGrid[i]) != grids.end())
			{
				vector<LPGAMEOBJECT>* temp = grids.at(camBoundGrid[i])->GetcoObjectList();
				coObjs.insert(coObjs.end(), temp->begin(), temp->end());
			}
		}
		vector<LPDYNAMICOBJECT>* changeGridObjs;
		for (int i = 0; i < camBoundGrid.size(); i++)
		{
			if (grids.find(camBoundGrid[i]) != grids.end())
			{
				changeGridObjs = grids.at(camBoundGrid[i])->Update(dt, &coObjs, mainPlayer->GetPosition().x, mainPlayer->GetPosition().y);
				for (int j = 0; j < changeGridObjs->size(); j++)
				{
					LPDYNAMICOBJECT obj = changeGridObjs->at(j);
					vector<int> objBoundGrids;
					objBoundGrids = this->GetBoundGrid(obj->GetBound());
					for (int k = 0; k < objBoundGrids.size(); k++)
					{
						int index = objBoundGrids[k];
						grids[index]->AddDynamicObj(obj);
					}
				}
				delete changeGridObjs;
			}
		}
		return mainPlayer->Update(dt, &coObjs);
	}
	mainPlayer->Update(dt);
	return 0;
}
void SectionBoss::Render()
{
	if (period == 0)
	{
		Rect cam = CGame::GetInstance()->GetCamBound();
		LPDIRECT3DTEXTURE9 texbg = CTextureManager::GetInstance()->Get(40);
		LPDIRECT3DTEXTURE9 texfg = CTextureManager::GetInstance()->Get(50);
		float bgX = cam.left + (cam.right - cam.left) / 2.0f;
		float bgY = cam.top + (cam.bottom - cam.top) / 2.0f;
		int R = rand() % 255;
		int G = rand() % 255;
		int B = rand() % 255;
		CGame::GetInstance()->Draw(bgX, bgY, texbg, cam.left, cam.top, cam.right, cam.bottom, -1, D3DCOLOR_XRGB(R, G, B));
		CGame::GetInstance()->Draw(bgX, bgY, texfg, cam.left, cam.top, cam.right, cam.bottom, -1, D3DCOLOR_XRGB(R, G, B));
	}
	if (period == 1)
	{
		vector<LPDYNAMICOBJECT>* objs = grids[0]->GetDynamicObj();
		for (int i = 0; i < objs->size(); i++) {
			objs->at(i)->Render();
		}
		mainPlayer->Render();
	}
	if (period == 2) {
		Rect cam = CGame::GetInstance()->GetCamBound();
		LPDIRECT3DTEXTURE9 texbg = CTextureManager::GetInstance()->Get(40);
		LPDIRECT3DTEXTURE9 texfg = CTextureManager::GetInstance()->Get(50);
		float bgX = cam.left + (cam.right - cam.left) / 2.0f;
		float bgY = cam.top + (cam.bottom - cam.top) / 2.0f;
		CGame::GetInstance()->Draw(bgX, bgY, texbg, cam.left, cam.top, cam.right, cam.bottom, -1, 255);
		Section::Render();
		mainPlayer->Render();
		CGame::GetInstance()->Draw(bgX, bgY, texfg, cam.left, cam.top, cam.right, cam.bottom, -1, 255);
	}
}