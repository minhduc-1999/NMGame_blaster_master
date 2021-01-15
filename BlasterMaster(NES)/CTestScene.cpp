#include "CTestScene.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Textures.h"
#include "Sprites.h"
#include "Brick.h"
#include "ResourceManager.h"
#include "CMagma.h"
#include "CGate.h"
#include "CLadder.h"
#include "OvwSectionTransition.h"
#include "AreaSectionTransition.h"
#include "SectionArea.h"
#include "SectionOvw.h"
#include "Jason.h"

using namespace std;

CTestScene::CTestScene(int id, string filePath, int type, D3DXVECTOR3 bg) :
	CScene(id, filePath, type, bg)
{
	key_handler = new CTestSceneKeyHandler(this);
	isSwitchingSection = false;
	transition = NULL;
	miniJason = NULL;
	sophia = NULL;
	current_section = -1;
	mainPlayer = NULL;
	hpBar = NULL;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define TEXTURE_BACKGROUND 40
#define TEXTURE_FOREGROUND 50

void CTestScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	string path = tokens[1];
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextureManager::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CTestScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut("[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSpriteManager::GetInstance()->Add(ID, tex, l, t, r, b);
}

void CTestScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimationManager::GetInstance()->Add(ani_id, ani);
}

void CTestScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimationManager* animations = CAnimationManager::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CTestScene::_ParseSection_SECTION(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	string path = tokens[1];
	LPSECTION section = NULL;
	if (this->type == 1)
		section = new SectionArea(id, path);
	else if (this->type == 2)
		section = new SectionOvw(id, path);
	if (section != NULL)
		sections[id] = section;
}

void CTestScene::_ParseSection_SOUND(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	string name = tokens[0];
	string path = tokens[1];

	Sound::getInstance()->loadSound(path.c_str(), name);
}

//Update render
int CTestScene::Update(DWORD dt)
{
	if (isSwitchingSection == false)
	{
		//CGame::GetInstance()->ProcessKeyboard();
		int result = sections[current_section]->Update(dt);
		if (result == 1)
			return result;
		//update camera
		D3DXVECTOR2 mainPos = mainPlayer->GetPosition();
		D3DXVECTOR2 mapPos = sections[current_section]->GetSectionMapPos();
		D3DXVECTOR2 mapDimen = sections[current_section]->GetSectionMapDimension();
		CGame::GetInstance()->UpdateCamera(mainPos, mapPos, mapDimen);
		Rect camPos = CGame::GetInstance()->GetCamBound();
		hpBar->Update(dt, camPos.left + 10, camPos.top + 170, mainPlayer->GetHP());
	}
	else
	{
		if (!transition->IsFinish())
		{
			transition->Update(dt);
		}
		else
		{
			CGame::GetInstance()->DisableKeyboard(false);
			isSwitchingSection = false;
			transition->Reset();
			current_section = transition->GetNextSectionId();
			sections[current_section]->Update(dt);
			mainPlayer = sections[current_section]->GetPlayer();
			D3DXVECTOR2 mainPos = mainPlayer->GetPosition();
			D3DXVECTOR2 mapPos = sections[current_section]->GetSectionMapPos();
			D3DXVECTOR2 mapDimen = sections[current_section]->GetSectionMapDimension();
			CGame::GetInstance()->UpdateCamera(mainPos, mapPos, mapDimen);
		}
	}
	return 0;
}

void CTestScene::Render()
{
	//Render background
	Rect cam = CGame::GetInstance()->GetCamBound();
	LPDIRECT3DTEXTURE9 texbg = CTextureManager::GetInstance()->Get(TEXTURE_BACKGROUND);
	LPDIRECT3DTEXTURE9 texfg = CTextureManager::GetInstance()->Get(TEXTURE_FOREGROUND);
	float bgX = cam.left + (cam.right - cam.left) / 2.0f;
	float bgY = cam.top + (cam.bottom - cam.top) / 2.0f;
	//CGame::GetInstance()->Draw(bgX, bgY, texbg, cam.left, cam.top, cam.right, cam.bottom, -1, 255);
	//Render object
	if (!isSwitchingSection)
	{
		sections[current_section]->Render();
	}
	else
		if (mainPlayer != NULL)
			mainPlayer->Render();
	//render foreground
	//CGame::GetInstance()->Draw(bgX, bgY, texfg, cam.left, cam.top, cam.right, cam.bottom, -1, 255);
	//render hpbar
	hpBar->Render();
}

/*
	Unload current scene
*/
void CTestScene::Unload()
{
	for (auto it = sections.begin(); it != sections.end(); ++it)
	{
		it->second->Unload();
		delete it->second;
	}
	sections.clear();
	if(mainPlayer != NULL)
		delete mainPlayer;
	if (sophia != NULL)
		delete sophia;
	if (miniJason != NULL)
		delete miniJason;
	if (transition != NULL)
		delete transition;
	if (hpBar != NULL)
		delete hpBar;
		
}

void CTestScene::SwitchSection(int section_id, D3DXVECTOR2 telePos)
{
	DebugOut("[INFO] Switching to section %d\n", section_id);
	CGame::GetInstance()->DisableKeyboard(true);
	if (section_id == -1)
		return;
	transition->Setsection(sections[current_section], sections[section_id], telePos, saveData);
	isSwitchingSection = true;
}

void CTestSceneKeyHandler::OnKeyDown(int KeyCode)
{
	if (scence->GetType() == 1)
	{
		if (KeyCode == DIK_C)
		{
			SaveData* saveData = scence->GetSaveData();
			if (((CTestScene*)scence)->GetPlayerType() == PLAYER_SOPHIA
				&& ((Sophia*)(((CTestScene*)scence)->GetPlayer()))->GetIsJumping() == false)
			{
				//save sophia state

				Sophia* sophia = ((Sophia*)(((CTestScene*)scence)->GetPlayer()));
				if (saveData == NULL)
				{
					saveData = new SaveData();
					scence->SetSaveData(saveData);
					saveData->sophiaX = sophia->GetPosition().x;
					saveData->sophiaY = sophia->GetPosition().y;
					saveData->sophiaSection = ((CTestScene*)scence)->GetCurrentSection();
					saveData->sophiaHP = sophia->GetHP();
					DebugOut("[INFO] Save Data last section = %d, %f, %f\n", saveData->sophiaSection, saveData->sophiaX, saveData->sophiaY);
				}
				
				

				((CTestScene*)scence)->GetPlayer()->OnKeyDown(DIK_C);
				((CTestScene*)scence)->addMiniJason();
				((CTestScene*)scence)->ChangePlayerType();
				Sound::getInstance()->play("Open", false, 1);
			}
			else if (((CTestScene*)scence)->GetPlayerType() == PLAYER_JASON
				&& ((MiniJason*)(((CTestScene*)scence)->GetPlayer()))->IsCollisionWithSophia())
			{
				//((CTestScene*)scence)->GetPlayerSophia()->OnKeyDown(DIK_C);

				//((CTestScene*)scence)->SetPlayer(((CTestScene*)scence)->GetPlayerSophia());
				((CTestScene*)scence)->GetPlayer()->OnKeyDown(DIK_C);
				((CTestScene*)scence)->deleteMiniJason();
				((CTestScene*)scence)->ChangePlayerType();
				Sound::getInstance()->play("Open", false, 1);
				if (saveData != NULL)
				{
					((CTestScene*)scence)->GetPlayer()->SetHP(saveData->sophiaHP);
					delete saveData;
					scence->SetSaveData(NULL);
				}
			}
			else
			{
				if (saveData != NULL && ((MiniJason*)((CTestScene*)scence)->GetPlayer())->IsCollisionWithSophia())
				{
					((CTestScene*)scence)->GetPlayer()->SetHP(saveData->sophiaHP);
					delete saveData;
					scence->SetSaveData(NULL);
				}
				return;
			}

		}
		if (KeyCode == DIK_Z)
		{
			if (((CTestScene*)scence)->GetPlayerType() == PLAYER_SOPHIA)
			{
				Sophia* currentPlayer = (Sophia*)(((CTestScene*)scence)->GetPlayer());
				if (currentPlayer->CanShoot())
					((CTestScene*)scence)->GetCurSection()->AddDynamicObject(currentPlayer->Shoot());
			}
			else if (((CTestScene*)scence)->GetPlayerType() == PLAYER_JASON)
			{
				MiniJason* currentPlayer = (MiniJason*)(((CTestScene*)scence)->GetPlayer());
				if (currentPlayer->CanShoot())
					((CTestScene*)scence)->GetCurSection()->AddDynamicObject(currentPlayer->Shoot());
			}
		}
	}
	else if(scence->GetType() == 2)
	{
		Jason* currentPlayer = (Jason*)(((CTestScene*)scence)->GetPlayer());
		currentPlayer->OnKeyDown(KeyCode);
		if (KeyCode == DIK_Z)
		{
			if (currentPlayer->CanShoot())
				((CTestScene*)scence)->GetCurSection()->AddDynamicObject(currentPlayer->Shoot());
		}
			
	}
	CDynamicGameObject* currentPlayer = ((CTestScene*)scence)->GetPlayer();
	currentPlayer->OnKeyDown(KeyCode);
}

void CTestSceneKeyHandler::KeyState(BYTE* states)
{
	//CGame* game = CGame::GetInstance();
	CDynamicGameObject* currentPlayer = ((CTestScene*)scence)->GetPlayer();
	currentPlayer->KeyState(states);
}

void CTestSceneKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	CDynamicGameObject* currentPlayer = ((CTestScene*)scence)->GetPlayer();
	currentPlayer->OnKeyUp(KeyCode);
}

void CTestScene::Load(D3DXVECTOR2 tlPos)
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
		case SCENE_SECTION_SECTION:
			_ParseSection_SECTION(line); break;
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
	switch (this->type)
	{
	case 1:
		this->transition = new AreaSectionTransition();
		break;
	case 2:
		this->transition = new OvwSectionTransition();
		break;
	case 3:
		break;
	default:
		break;
	}

	DebugOut("[INFO] Loading section file : %s has been loaded successfully\n", sceneFilePath);

	//SwitchSection(current_section, D3DXVECTOR2(-1, -1));
	SwitchSection(current_section, tlPos);
	this->mainPlayer = sections[current_section]->GetPlayer();
	hpBar = new HPBar();
}


