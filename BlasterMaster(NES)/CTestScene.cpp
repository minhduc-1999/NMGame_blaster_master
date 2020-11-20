#include "CTestScene.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Textures.h"
#include "Sprites.h"
#include "tinyxml.h"
#include "Brick.h"
#include "ResourceManager.h"
#include "CMagma.h"
#include "CGate.h"
#include "CLadder.h"

using namespace std;

CTestScene::CTestScene(int id, string filePath) :
	CScene(id, filePath)
{
	key_handler = new CTestSceneKeyHandler(this);
	mapX = 0.0f;
	mapY = 0.0f;
	mapWidth = 1024.0f;
	mapHeight = 240.0f;
}

CTestScene::CTestScene() :
	CScene()
{
	key_handler = new CTestSceneKeyHandler(this);
	gridHeight = 160.0f;
	gridWidth = 256.0f;
	mapX = 0.0f;
	mapY = 0.0f;
	mapWidth = 512.0f;
	mapHeight = 1280.0f;
	gridRow = mapHeight / gridHeight;
	gridCol = mapWidth / gridWidth;

	for (int i = 0; i < gridRow; i++)
		for (int j = 0; j < gridCol; j++)
		{
			LPGRID temp = new Grid(i, j);
			int index = (i % gridRow) * gridCol + j % gridCol;
			grids.emplace(index, temp);
		}
} 

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_STATIC_OBJECTS	6
#define SCENE_SECTION_DYNAMIC_OBJECTS	7
#define SCENE_SECTION_MAP	8

#define OBJECT_TYPE_SOPHIA	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_ARROW	2
#define	OBJECT_TYPE_SKULL	10
#define OBJECT_TYPE_MINE	11
#define	OBJECT_TYPE_TELEPORTER 12

#define MAX_SCENE_LINE 1024

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

/*
	Parse a line in section [OBJECTS]
*/
void CTestScene::_ParseSection_DYNAMIC_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CDynamicGameObject* obj = NULL;

	switch (object_type)
	{
		//dynamic obj
	case OBJECT_TYPE_SOPHIA:
		if (mainPlayer != NULL)
		{
			DebugOut("[ERROR] main object was created before!\n");
			return;
		}
		obj = new Sophia(x, y);
		mainPlayer = (Sophia*)obj;

		DebugOut("[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_SKULL:
		obj = new Skull(x, y);
		break;
	case OBJECT_TYPE_MINE:
		obj = new Mine(x, y);
		break;
	case OBJECT_TYPE_TELEPORTER:
		obj = new Teleporter(x, y);
		break;
	default:
		DebugOut("[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	dynamicObj.push_back(obj);
}

void CTestScene::_ParseSection_STATIC_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	CStaticGameObject* obj = NULL;

	switch (object_type)
	{
		//static obj
	case OBJECT_TYPE_BRICK:
		obj = new Brick(x, y);
		break;
		/*case OBJECT_TYPE_GATE:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;*/
	default:
		DebugOut("[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	for (int i = 3; i < tokens.size(); i++)
	{
		int spriteID = atoi(tokens[i].c_str());
		obj->AddSprite(CSpriteManager::GetInstance()->Get(spriteID));
	}
	staticObj.push_back(obj);
}

//Update render
void CTestScene::Update(DWORD dt)
{
	/*vector<LPGAMEOBJECT> coObjs;
	Rect camPos = CGame::GetInstance()->GetCamBound();
	D3DXVECTOR2 grid = GetBoundGrid(camPos);
	for (int i = (int)grid.x; i <= (int)grid.y; i++)
	{
		if (grids.find(i) != grids.end())
		{
			vector<LPSTATICOBJECT>* temp = grids.at(i)->GetcoObjectList();
			coObjs.insert(coObjs.end(), temp->begin(), temp->end());
		}
	}
	
	mainPlayer->Update(dt, &coObjs);*/
	vector<LPGAMEOBJECT> coObjs;
	coObjs.insert(coObjs.end(), staticObj.begin(), staticObj.end());
	/*for (int i = 0; i < dynamicObj.size(); i++)
		dynamicObj[i]->Update(dt, &coObjs);*/
	mainPlayer->Update(dt, &coObjs);
	float cx, cy;
	cx = mainPlayer->GetPosition().x;
	cy = mainPlayer->GetPosition().y;

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx, cy, D3DXVECTOR3{ mapX, mapY, 0 }, D3DXVECTOR3{ mapWidth, mapHeight, 0 });
}

void CTestScene::Render()
{
	//Kiem tra xem camera chiem grid nao
	//Rect camPos = CGame::GetInstance()->GetCamBound();
	//D3DXVECTOR2 grid = GetBoundGrid(camPos);
	//for (int i = (int)grid.x; i <= (int)grid.y; i++)
	//{
	//	if (grids.find(i) != grids.end())
	//		grids.at(i)->Render();
	//}
	////render main
	////player->Render();
	//if (mainPlayer != sophia)
	//{
	//	sophia->Render();
	//}
	//mainPlayer->Render();
	
	for (int i = 0; i < staticObj.size(); i++)
		staticObj[i]->Render();
	mainPlayer->Render();
	//for (int i = 0; i < dynamicObj.size(); i++)
	//	dynamicObj[i]->Render();
}

/*
	Unload current scene
*/
void CTestScene::Unload()
{

}

D3DXVECTOR2 CTestScene::GetBoundGrid(Rect bound)
{
	//	D3DXVECTOR2 start(startRow, startCol,  0);
	D3DXVECTOR2 start(bound.top / gridHeight, bound.left / gridWidth);
	//	D3DXVECTOR2 end(endRow, endCol,  0);
	D3DXVECTOR2 end(bound.bottom / gridHeight, bound.right / gridWidth);

	int startGrid = ((int)start.x % gridRow) * gridCol + (int)start.y % gridCol;
	int endGrid = ((int)end.x % gridRow) * gridCol + (int)end.y % gridCol;

	return D3DXVECTOR2(startGrid, endGrid);
}


void CTestSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	if (KeyCode == DIK_C)
	{
		if (((CTestScene*)scence)->GetPlayerType() == PLAYER_SOPHIA
			&& ((CTestScene*)scence)->GetPlayerSophia()->GetIsJumping() == false)
		{
			((CTestScene*)scence)->GetPlayerSophia()->OnKeyDown(DIK_C);

			((CTestScene*)scence)->GetPlayerJason()->SetPosition(((CTestScene*)scence)->GetPlayer()->GetPosition().x, ((CTestScene*)scence)->GetPlayer()->GetPosition().y - 10);
			((CTestScene*)scence)->SetPlayer(((CTestScene*)scence)->GetPlayerJason());
			((CTestScene*)scence)->ChangePlayerType();
		}
		else if (((CTestScene*)scence)->GetPlayerType() == PLAYER_JASON
			/*&&		((CTestScene*)scence)->GetPlayerJason()->IsCollisionWithSophia()*/)
		{
			((CTestScene*)scence)->GetPlayerSophia()->OnKeyDown(DIK_C);

			((CTestScene*)scence)->SetPlayer(((CTestScene*)scence)->GetPlayerSophia());
			((CTestScene*)scence)->ChangePlayerType();

		}
		else
		{
			return;
		}

	}
	CDynamicGameObject* currentPlayer = ((CTestScene*)scence)->GetPlayer();
	currentPlayer->OnKeyDown(KeyCode);
}

void CTestSceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CDynamicGameObject* currentPlayer = ((CTestScene*)scence)->GetPlayer();
	currentPlayer->KeyState(states);
}

void CTestSceneKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	CDynamicGameObject* currentPlayer = ((CTestScene*)scence)->GetPlayer();
	currentPlayer->OnKeyUp(KeyCode);
}

void CTestScene::GetMapInfo(string path)
{
	TiXmlDocument doc("mapArea2.xml");
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
		return;
	}
	TiXmlElement* root = doc.RootElement();
	TiXmlElement* data = root->FirstChildElement()
		->NextSiblingElement()
		->NextSiblingElement()
		->FirstChildElement();
	string s = data->GetText();

	std::string delimiter = ",";
	size_t pos = 0;
	std::string token;
	int i = 0, j = 0;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		//std::cout << token << std::endl;
		map[i][j] = stoi(token);
		s.erase(0, pos + delimiter.length());
		j++;
		if (i == 128)
			i = 0;
		if (j == 128)
		{
			j = 0;
			i++;
		}
	}
}

void CTestScene::Load()
{
	DebugOut("[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);
	string line;
	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
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
		if (line == "[STATIC_OBJECTS]") {
			section = SCENE_SECTION_STATIC_OBJECTS; continue;
		}
		if (line == "[DYNAMIC_OBJECTS]") {
			section = SCENE_SECTION_DYNAMIC_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_DYNAMIC_OBJECTS: _ParseSection_DYNAMIC_OBJECTS(line); break;
		case SCENE_SECTION_STATIC_OBJECTS: _ParseSection_STATIC_OBJECTS(line); break;
		}
	}

	f.close();

	//CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut("[INFO] Done loading scene resources %s\n", sceneFilePath);
}


