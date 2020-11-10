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

CTestScene::CTestScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CTestSceneKeyHandler(this);
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

//#define SCENE_SECTION_UNKNOWN -1
//#define SCENE_SECTION_TEXTURES 2
//#define SCENE_SECTION_SPRITES 3
//#define SCENE_SECTION_ANIMATIONS 4
//#define SCENE_SECTION_ANIMATION_SETS	5
//#define SCENE_SECTION_OBJECTS	6
//#define SCENE_SECTION_MAP	7
//
//#define OBJECT_TYPE_MARIO	0
//#define OBJECT_TYPE_BRICK	1
//#define OBJECT_TYPE_GOOMBA	2
//#define OBJECT_TYPE_KOOPAS	3
//
//#define OBJECT_TYPE_PORTAL	50
//
//#define MAX_SCENE_LINE 1024

void CTestScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjs;
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

	player->Update(dt, &coObjs);

	float cx, cy;
	cx = player->GetPosition().x;
	cy = player->GetPosition().y;

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx, cy, D3DXVECTOR3{ mapX, mapY, 0 }, D3DXVECTOR3{ mapWidth, mapHeight, 0 });
}

void CTestScene::Render()
{
	//Kiem tra xem camera chiem grid nao
	Rect camPos = CGame::GetInstance()->GetCamBound();
	D3DXVECTOR2 grid = GetBoundGrid(camPos);
	for (int i = (int)grid.x; i <= (int)grid.y; i++)
	{
		if (grids.find(i) != grids.end())
			grids.at(i)->Render();
	}
	//render main
	player->Render();
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
	Player* player = ((CTestScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_X:
		if (player->GetNX() == 1)
		{
			player->SetState(SOPHIA_STATE_JUMP_RIGHT);
		}
		else
		{
			player->SetState(SOPHIA_STATE_JUMP_LEFT);
		}
		break;
	}
}

void CTestSceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	Player* player = ((CTestScene*)scence)->GetPlayer();
	if (player->GetState() == -1) return;
	if (game->IsKeyDown(DIK_RIGHT))
		if (player->GetNX() == -1)
		{
			player->SetState(SOPHIA_STATE_TURN_RUN);
		}
		else
		{
			player->SetState(SOPHIA_STATE_RUN_RIGHT);
		}
	else if (game->IsKeyDown(DIK_LEFT))
		if (player->GetNX() == 1)
		{
			player->SetState(SOPHIA_STATE_TURN_RUN);
		}
		else
		{
			player->SetState(SOPHIA_STATE_RUN_LEFT);
		}
		//player->SetState(SOPHIA_STATE_RUN_LEFT);
	/*else if (game->IsKeyDown(DIK_UP))
		player->SetState(GO_TOP);
	else if (game->IsKeyDown(DIK_DOWN))
		player->SetState(GO_BOTTOM);*/
	else
	{
		if (player->GetNX() == 1)
		{
			player->SetState(SOPHIA_STATE_IDLE_RIGHT);
		}
		else
		{
			player->SetState(SOPHIA_STATE_IDLE_LEFT);
		}
	}

}

void CTestSceneKeyHandler::OnKeyUp(int KeyCode)
{

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
	GetMapInfo("mapArea2.xml");
	/*Insect* insect;
	Orb* orb;
	Jumper2* jumper2;*/
	Player* main;
	main = new Player(256, 256);


	//Load object
	//tinh so luong tile cua map ma scene nay co
	int numTileX = mapWidth / 16;
	int numTileY = mapHeight / 16;

	//tinh xem tile dau tien co chi so bao nhieu
	int startTileX = mapX / 16;
	int startTileY = mapY / 16;

	for (int i = startTileY; i < startTileY + numTileY; i++)
		for (int j = startTileX; j < startTileX + numTileX; j++)
		{
			int t = map[i][j];
			LPSTATICOBJECT obj = NULL;
			LPBACKGROUND bg = NULL;
			switch (t)
			{
				//object brick
			case 7:	case 61: case 62:case 63:case 64:case 65:case 66:case 117:
			case 51:
			{
				if (i == 0 && j == 31)
					i += 0;
				obj = new Brick(float(j * 16 + 8), float(i * 16 + 8));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t - 1));
				break;
			}
			//object magma
			case 107:
			{
				obj = new CMagma(float(j * 16 + 8), float(i * 16 + 8));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t - 1));
				break;
			}
			//object gate
			case 46:case 48:
			{
				obj = new CGate(float(j * 16 + 16), float(i * 16 + 16));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t - 1));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t + 10));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t + 11));
				break;
			}
			//remain part of gate
			case 47:case 57:case 58: case 49: case 59: case 60:
				break;
			//object ladder
			case 8:
			{
				obj = new CLadder(float(j * 16 + 8), float(i * 16 + 8));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t - 1));
				break;
			}
			//none object (background)
			default:
			{
				bg = new CBackground(float(j * 16 + 8), float(i * 16 + 8));
				bg->SetSprite(CSpriteManager::GetInstance()->Get(20000 + t - 1));
			}
			}
			if (obj != NULL)
			{
				//xac dinh cac grid chua object
				D3DXVECTOR2 grid = GetBoundGrid(obj->GetBound());
				for (int m = (int)grid.x; m <= (int)grid.y; m++)
				{
					if (grids.find(m) != grids.end())
						grids.at(m)->AddObject(obj);
				}
			}
			if (bg != NULL)
			{
				//xac dinh cac grid chua object
				D3DXVECTOR2 grid = GetBoundGrid(bg->GetBound());
				for (int m = (int)grid.x; m <= (int)grid.y; m++)
				{
					if (grids.find(m) != grids.end())
						grids.at(m)->AddBackgroundTile(bg);
				}
			}

		}
	main->AddAnimation(100);
	main->AddAnimation(101);
	main->AddAnimation(102);
	main->AddAnimation(103);
	main->AddAnimation(104);
	main->SetPosition(64, 100);
	main->SetState(SOPHIA_STATE_RUN_RIGHT);
	player = main;
	/*insect = new Insect();
	insect->AddAnimation(500);
	insect->SetPosition(100, 100);
	insect->SetState(INSECT_STATE_FLYING_RIGHT);

	orb = new Orb();
	orb->AddAnimation(510);
	orb->AddAnimation(511);
	orb->SetPosition(100, 250);
	orb->SetState(ORB_STATE_RUNNING_RIGHT);

	jumper2 = new Jumper2();
	jumper2->AddAnimation(520);
	jumper2->SetPosition(100, 350);
	jumper2->SetState(JUMPER2_STATE_WALKING_LEFT);*/
}


