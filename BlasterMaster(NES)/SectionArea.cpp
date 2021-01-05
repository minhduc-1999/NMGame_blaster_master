#include "SectionArea.h"
#include "MiniJason.h"

void SectionArea::Load(SaveData* data)
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
				mainPlayer = new MiniJason(defaultPos.x, defaultPos.y);
				mainPlayer->SetTeam(0);
				mainPlayer->SetType(2);
				mainPlayer->SetAnimationSet(ani_setMINIJASON);
			}
			DebugOut("[INFO] Loaded save data %d\n");
		}
	}
	if (mainPlayer != NULL)
	{
		//D3DXVECTOR2 pos = mainPlayer->GetPosition();
		//DebugOut("[Pos player trans before load]\tx: %f, y: %f\n", pos.x, pos.y);
		DebugOut("[ERROR] main object was created before in area!\n");
		return;
	}
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(1);
	mainPlayer = new Sophia(defaultPos.x, defaultPos.y);
	mainPlayer->SetAnimationSet(ani_set);
	mainPlayer->SetTeam(0);
	mainPlayer->SetType(1);
	DebugOut("[INFO] sophia created!\n");
	//DebugOut("[PLAYER POSITION]\t%f\t%f\n", x, y);
}