#include "IntroScene.h"
#include "Utils.h"
#include <fstream>
#include "Textures.h"

IntroScene::IntroScene(int id, string filePath, int type, D3DXVECTOR3 bg) : CScene(id, filePath, type, bg)
{
	this->key_handler = new IntroSceneKeyHandler(this);
	current_intro = -1;
	introSets = NULL;
	start = GetTickCount64();
}

void IntroScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 5)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		int R = atoi(tokens[i + 2].c_str());
		int G = atoi(tokens[i + 3].c_str());
		int B = atoi(tokens[i + 4].c_str());
		D3DCOLOR color = D3DCOLOR_XRGB(R, G, B);
		ani->AddWithColor(sprite_id, frame_time, color);
	}

	CAnimationManager::GetInstance()->Add(ani_id, ani);
}

void IntroScene::Load(D3DXVECTOR2 tlPos)
{
	DebugOut("[INFO] Start loading scene resources from : %s \n", sceneFilePath);

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

	DebugOut("[INFO] Intro has been loaded successfully\n", sceneFilePath);
	current_intro = 0;
	introSets = CAnimationSets::GetInstance()->Get(1);
}

void IntroScene::Unload()
{
	
	
}
int IntroScene::Update(DWORD dt)
{
	switch (current_intro)
	{
	case 0:
		if (GetTickCount64() - start >= INTRO_SPAN_MIN)
			NextIntro();
		break;
	case 1:
		if (introSets->at(current_intro)->IsCompleted())
			NextIntro();
		break;
	case 2:
		if (GetTickCount64() - start >= 4000)
			NextIntro();
		break;
	case 3:
		if (introSets->at(current_intro)->IsCompleted())
			CGame::GetInstance()->SwitchScene(2, 1, D3DXVECTOR2(-1, -1));
	default:
		break;
	}
	return 0;
}

void IntroScene::Render()
{
	if (current_intro == -1)
		return;
	Rect cam = CGame::GetInstance()->GetCamBound();
	LPDIRECT3DTEXTURE9 texbg = CTextureManager::GetInstance()->Get(0);
	float bgX = cam.left + (cam.right - cam.left) / 2.0f;
	float bgY = cam.top + (cam.bottom - cam.top) / 2.0f;
	switch (current_intro)
	{
	case 0:
		CGame::GetInstance()->Draw(bgX, bgY, texbg, cam.left, cam.top, cam.right, cam.bottom, -1, 255);
		introSets->at(current_intro)->RenderWithColor(bgX + 2, bgY - 20, -1);
		break;
	default:
		introSets->at(current_intro)->RenderWithColor(bgX, bgY, -1);
		break;
	}
}
void IntroScene::NextIntro()
{
	current_intro++;
	start = GetTickCount64();
	if (current_intro >= introSets->size())
		CGame::GetInstance()->SwitchScene(2, 1, D3DXVECTOR2(-1, -1));
}
void IntroSceneKeyHandler::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_RETURN)
	{
		((IntroScene*)scence)->NextIntro();
	}
}