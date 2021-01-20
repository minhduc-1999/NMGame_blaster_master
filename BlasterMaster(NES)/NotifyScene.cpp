#include "NotifyScene.h"
#include "Utils.h"
#include <fstream>
#include "NotifyPanel.h"
#include "SelectPanel.h"

NotifyScene::NotifyScene(int id, string filePath, int type, D3DXVECTOR3 bg) : CScene(id, filePath, type, bg)
{
	this->key_handler = new NotifySceneKeyHandler(this);
	current_panel = -1;
}
void NotifyScene::SetNotify(int mainLives)
{
	if (mainLives == 0)
	{
		current_panel = 3;
		saveData->lastScene = SCENE_AREA;
		saveData->lastSection = 1;
	}
		
	else {
		current_panel = mainLives - 1;
		if (current_panel < 0)
			current_panel = 0;
	}
		
}
void NotifyScene::Load(D3DXVECTOR2 tlPos)
{
	DebugOut("[INFO] Start loading notify scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);
	string line;
	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;
	while (getline(f, line))
	{
		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[SECTIONS]") {
			section = SCENE_SECTION_SECTION; continue;
		}
		if (line == "[SOUNDS]") {
			section = SCENE_SECTION_SOUND; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS:
			_ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_SOUND:
			_ParseSection_SOUND(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}
	f.close();

	DebugOut("[INFO] notify scene has been loaded successfully\n");
	panels[0] = new NotifyPanel(127.0f, 111.0f, 0.0f, 0.0f, 101, 16, CSpriteManager::GetInstance()->Get(7000));
	panels[1] = new NotifyPanel(127.0f, 111.0f, 0.0f, 0.0f, 101, 16, CSpriteManager::GetInstance()->Get(7001));
	panels[2] = new NotifyPanel(127.0f, 111.0f, 0.0f, 0.0f, 101, 16, CSpriteManager::GetInstance()->Get(7002));

	unordered_map<int, CSprite*> sp;
	sp[0] = CSpriteManager::GetInstance()->Get(8000);
	sp[1] = CSpriteManager::GetInstance()->Get(8001);

	panels[3] = new SelectPanel(127.0f, 111.0f, 0.0f, 0.0f, 101, 16, sp);
}
void NotifyScene::Unload()
{

}
int NotifyScene::Update(DWORD dt)
{
	if (current_panel == panels.size() || current_panel == -1)
		return 0;
	if (panels[current_panel]->IsDone())
	{
		/*saveData->sophiaX = -1;
		saveData->sophiaY = -1;
		saveData->sophiaSection = -1;
		saveData->sophiaHP = -1;
		saveData->sophiaState = -1;*/
		CGame::GetInstance()->SwitchScene(saveData->lastScene, saveData->lastSection, D3DXVECTOR2(-1, -1));
	}
		
	panels[current_panel]->Update(dt);
	//
	return 1;
}
void NotifyScene::Render()
{
	if (current_panel == panels.size() || current_panel == -1)
		return;
	panels[current_panel]->Render();
}

void NotifySceneKeyHandler::OnKeyDown(int KeyCode)
{
	((NotifyScene*)scence)->GetCurPanel()->OnKeyDown(KeyCode);
}