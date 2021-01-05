#include "SectionOvw.h"
#include "Jason.h"
void SectionOvw::Load(SaveData* data)
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
	mainPlayer = new Jason(defaultPos.x, defaultPos.y);
	mainPlayer->SetAnimationSet(ani_set);
	mainPlayer->SetTeam(0);
	mainPlayer->SetType(1);
	DebugOut("[INFO] Jason Created!\n");
	//DebugOut("[PLAYER POSITION]\t%f\t%f\n", x, y);
}