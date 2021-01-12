#include "EndGameScene.h"
#include "Utils.h"
#include <fstream>

EndGameScene::EndGameScene(int id, string filePath, int type, D3DXVECTOR3 bg) : CScene(id, filePath, type, bg)
{
	this->key_handler = new EndGameKeyHandler(this);
	current_period = -1;
	endGameSets = NULL;
	start = GetTickCount64();
}
void EndGameScene::Load(D3DXVECTOR2 tlPos)
{
	DebugOut("[INFO] Start loading end game scene resources from : %s \n", sceneFilePath);

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
	//Sound::getInstance()->play("lvl2", true, 0);
	f.close(); 

	DebugOut("[INFO] End game scene has been loaded successfully\n", sceneFilePath);

	endGameSets = CAnimationSets::GetInstance()->Get(0);
}
void EndGameScene::Unload()
{

}
int EndGameScene::Update(DWORD dt)
{
	return 1;
}
void EndGameScene::Render()
{
 	endGameSets->at(0)->Render(127, 111, -1);
	CSpriteManager::GetInstance()->Get(6000)->Draw(132, 124, -1, 255);
	/*for (int i = 65; i <= 90; i++)
	{
		CSpriteManager::GetInstance()->Get(i)->Draw(8 * (i - 60) + 2, 50, -1, 255);
	}
	for (int i = 48; i <= 57; i++)
	{
		CSpriteManager::GetInstance()->Get(i)->Draw(8 * (i - 48) + 2, 60, -1, 255);
	}*/
	/*CSpriteManager::GetInstance()->Get(45)->Draw(0, 70, -1, 255);
	CSpriteManager::GetInstance()->Get(46)->Draw(10, 70, -1, 255);
	CSpriteManager::GetInstance()->Get(33)->Draw(20, 70, -1, 255);*/
	//CAnimationManager::GetInstance()->Get(2)->Render(160, 95, -1);

}

void EndGameScene::NextIntro()
{

}

//void EndGameScene::_ParseSection_ANIMATIONS(string line)
//{
//
//}