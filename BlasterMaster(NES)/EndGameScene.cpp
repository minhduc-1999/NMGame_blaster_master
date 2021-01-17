#include "EndGameScene.h"
#include "Utils.h"
#include <fstream>
#include "DescPanel.h"
#include "SlidePanel.h"
#include "MoviePanel.h"

EndGameScene::EndGameScene(int id, string filePath, int type, D3DXVECTOR3 bg) : CScene(id, filePath, type, bg)
{
	this->key_handler = new EndGameKeyHandler(this);
	current_period = 0;
	panels[2] = new DescPanel(127.0f, 224.0f, 0.0f, -0.05f);
}
void EndGameScene::_ParseSection_DESCRIPTION(string path)
{
	DebugOut("[INFO] Start loading description resources from : %s \n", path);

	ifstream f;
	f.open(path);
	string line;
	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;
	while (getline(f, line))
	{
		if (line[0] == '#') continue;	// skip comment lines	
		((DescPanel*)(panels[2]))->PushDesc(line);
	}
	f.close();
	DebugOut("[INFO] Description has been loaded successfully\n");
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
		if (line == "[DESCRIPTION]") {
			section = SCENE_SECTION_DESC; continue;
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
		case SCENE_SECTION_DESC: _ParseSection_DESCRIPTION(line); break;
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

	DebugOut("[INFO] End game scene has been loaded successfully\n");
	panels[0] = new SlidePanel(128.0f, 111.0f, -0.05f, 0.0f, 512, 224, CAnimationManager::GetInstance()->Get(2));
	panels[1] = new MoviePanel(256.0f, 111.0f, -0.05f, 0.0f, 512, 224, CSpriteManager::GetInstance()->Get(1000));

}
void EndGameScene::Unload()
{

}
int EndGameScene::Update(DWORD dt)
{
	if (current_period == panels.size())
		return 0;
	if (panels[current_period]->IsDone())
	{
		NextIntro();
		return 0;
	}
	panels[current_period]->Update(dt);
	//
	return 1;
}
void EndGameScene::Render()
{
	//endGameSets->at(0)->Render(127, 111, -1);
	//CSpriteManager::GetInstance()->Get(6000)->Draw(132, 124, -1, 255);
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
	if (current_period == panels.size())
		return;
	if (current_period == 2)
	{
		int n = 224 / 7 + 1;
		for (int i = 0; i <= n; i++)
		{
			CSpriteManager::GetInstance()->Get(4001)->Draw(64, i * 7 + 3, -1, 255);
		}
		//render logo
		CSpriteManager::GetInstance()->Get(4000)->Draw(64, 111, -1, 255);
	}


	panels[current_period]->Render();

	////rener column


	//descPanel->Render();
}

void EndGameScene::NextIntro()
{
	current_period++;
	if (current_period == 2)
		this->background = D3DCOLOR_XRGB(0, 0, 0);
}

//void EndGameScene::_ParseSection_ANIMATIONS(string line)
//{
//
//}