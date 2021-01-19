#include "SectionBoss.h"
#include "Jason.h"
#include "Textures.h"
#include "Boss.h"
void SectionBoss::Load(SaveData* data, D3DXVECTOR2 mainPos)
{
	Section::Load();
	startTime = GetTickCount64();
	if (mainPlayer != NULL)
	{
		//D3DXVECTOR2 pos = mainPlayer->GetPosition();
		//DebugOut("[Pos player trans before load]\tx: %f, y: %f\n", pos.x, pos.y);
		DebugOut("[ERROR] main object was created before in ovw!\n");
		return;
	}
}

int SectionBoss::Update(DWORD dt)
{
	if (period == 0)
	{
		if (GetTickCount64() - startTime > 7000)
		{
			period = 1;
			Boss* boss = new Boss(910, 590, 41, 42);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(40);
			boss->SetAnimationSet(ani_set);
			boss->SetState(BOSS_STATE_FLYING);
			grids[0]->AddDynamicObj(boss);
		}
	}
	if (period == 1) {
		vector<LPGAMEOBJECT>* temp = grids.at(0)->GetcoObjectList();
		grids[0]->Update(dt, temp, mainPlayer->GetPosition().x, mainPlayer->GetPosition().y);

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
}