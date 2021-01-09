#include "SectionArea.h"
#include "MiniJason.h"

void SectionArea::Load(SaveData* data, D3DXVECTOR2 mainPos)
{
	Section::Load();
	if (data != NULL)
	{
		if (data->sophiaSection == id)
		{
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(1);
			LPDYNAMICOBJECT obj;
			obj = new Sophia(data->sophiaX, data->sophiaY);
			obj->SetAnimationSet(ani_set);
			obj->SetTeam(0);
			obj->SetType(1);
			vector<int> gridPos = GetBoundGrid(obj->GetBound());
			for (int i = 0; i < gridPos.size(); i++)
			{
				grids[gridPos[i]]->AddDynamicObj(obj);
			}
			if (mainPlayer == NULL)
			{
				LPANIMATION_SET ani_setMINIJASON = animation_sets->Get(2);
				if(mainPos.x == -1 && mainPos.y == -1)
					mainPlayer = new MiniJason(defaultPos.x, defaultPos.y);
				else
					mainPlayer = new MiniJason(mainPos.x, mainPos.y);
				mainPlayer->SetTeam(0);
				mainPlayer->SetType(2);
				mainPlayer->SetState(MINIJASON_STATE_IDLE_LEFT);
				mainPlayer->SetAnimationSet(ani_setMINIJASON);
				DebugOut("[AREA-INFO] create minijason\n");
				DebugOut("[AREA-INFO] Main Pos:\t%f\t%f\n", mainPlayer->GetPosition().x, mainPlayer->GetPosition().y);
			}
			DebugOut("[AREA-INFO] Loaded save data\n");
		}
	}
	else
	{
		if (mainPlayer != NULL)
		{
			//D3DXVECTOR2 pos = mainPlayer->GetPosition();
			//DebugOut("[Pos player trans before load]\tx: %f, y: %f\n", pos.x, pos.y);
			DebugOut("[ERROR] main object was created before in area! && no save data\n");
			return;
		}
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(1);
		if (mainPos.x == -1 && mainPos.y == -1)
			mainPlayer = new Sophia(defaultPos.x, defaultPos.y);
		else
			mainPlayer = new Sophia(mainPos.x, mainPos.y);
		mainPlayer->SetAnimationSet(ani_set);
		mainPlayer->SetTeam(0);
		mainPlayer->SetType(1);
		mainPlayer->SetState(SOPHIA_STATE_IDLE_RIGHT);
		DebugOut("[AREA-INFO] sophia created!\n");
		DebugOut("[AREA-INFO] Main Pos:\t%f\t%f\n", mainPlayer->GetPosition().x, mainPlayer->GetPosition().y);
	}
}