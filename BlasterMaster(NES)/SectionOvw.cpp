#include "SectionOvw.h"
#include "Jason.h"
void SectionOvw::Load(SaveData* data, D3DXVECTOR2 mainPos)
{
	Section::Load();
	if (mainPlayer != NULL)
	{
		//D3DXVECTOR2 pos = mainPlayer->GetPosition();
		//DebugOut("[Pos player trans before load]\tx: %f, y: %f\n", pos.x, pos.y);
		DebugOut("[ERROR] main object was created before in ovw!\n");
		return;
	}
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(3);
	if (mainPos.x == -1 && mainPos.y == -1)
		mainPlayer = new Jason(defaultPos.x, defaultPos.y);
	else
		mainPlayer = new Jason(mainPos.x, mainPos.y);
	mainPlayer->SetAnimationSet(ani_set);
	mainPlayer->SetTeam(0);
	mainPlayer->SetType(1);
	mainPlayer->SetState(JASON_STATE_DIE);
	DebugOut("[INFO] Jason Created!\n");
	DebugOut("[OVW-INFO] Main Pos:\t%f\t%f\n", mainPlayer->GetPosition().x, mainPlayer->GetPosition().y);
}