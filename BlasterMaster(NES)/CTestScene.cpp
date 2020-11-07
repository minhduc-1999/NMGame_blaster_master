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
/*
#include "Utils.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Brick.h"
#include "Portal.h"*/

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
	gridHeight = 320.0f;
	gridWidth = 512.0f;
	mapX = 0.0f;
	mapY = 0.0f;
	mapWidth = 512.0f;
	mapHeight = 1280.0f;
	gridRow = mapHeight / gridHeight;
	gridCol = mapWidth / gridWidth;

	for (int i = 0; i < gridRow; i++)
		for (int j = 0; j < gridCol; j++)
		{
			unordered_map<int, LPGRID> temp({ { j, new Grid(i, j)} });
			grids.emplace(i, temp);
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
	player->Update(dt);
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
	//tinh so luong tile cua map ma scene nay co
	int numTileX = mapWidth / 16;
	int numTileY = mapHeight / 16;

	//tinh xem tile dau tien co chi so bao nhieu
	int startTileX = mapX / 16;
	int startTileY = mapY / 16;
	//Draw background
	for (int i = startTileY; i < startTileY + numTileY; i++)
		for (int j = startTileX; j < startTileX + numTileX; j++)
		{
			int t = map[i][j];
			switch (t)
			{
				//static object: tuong tac duoc nhung khong the di chuyen
			case 46: case 47: case 48: case 49:
			case 57: case 58: case 59: case 60:
			case 8:
			case 107:
				//brick
			case 7:
			case 62:
			case 63:
			case 64:
			case 65:
			case 66:
			case 117:
			case 51:
				break;
			default:
				int x = j * 16 + 8, y = i * 16 + 8;
				LPSPRITE sprite = CSpriteManager::GetInstance()->Get(20000 + t - 1);
				if (sprite == NULL)
					break;
				sprite->Draw(x, y, -1);
				break;
			}
		}

	/*for (int i = 0; i < staticObject.size(); i++)
		staticObject[i]->Render();*/

		//Kiem tra xem camera chiem grid nao
	RECT camPos = CGame::GetInstance()->GetCamBound();
	//	D3DXVECTOR3 start(startRow, startCol,  0);
	D3DXVECTOR3 start(camPos.top / gridHeight, camPos.left / gridWidth, 0);
	//	D3DXVECTOR3 end(endRow, endCol,  0);
	D3DXVECTOR3 end(camPos.bottom / gridHeight, camPos.right / gridWidth, 0);
	for (int i = (int)start.x; i <= (int)end.x; i++)
		for (int j = (int)start.y; j <= (int)end.y; j++)
		{
			if (grids.find(i) != grids.end())
				if (grids.at(i).find(j) != grids.at(i).end())
					grids.at(i).at(j)->Render();
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

void CTestSceneKeyHandler::OnKeyDown(int KeyCode)
{

}

void CTestSceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	Enemy1* player = ((CTestScene*)scence)->GetPlayer();
	if (player->GetState() == -1) return;
	if (game->IsKeyDown(DIK_RIGHT))
		player->SetState(GO_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		player->SetState(GO_LEFT);
	else if (game->IsKeyDown(DIK_UP))
		player->SetState(GO_TOP);
	else if (game->IsKeyDown(DIK_DOWN))
		player->SetState(GO_BOTTOM);
	else
		player->SetState(IDLE);

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
	Enemy1* main;
	main = new Enemy1(256, 256);


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
			switch (t)
			{
				//object brick
			case 7:	case 61: case 62:case 63:case 64:case 65:case 66:case 117:
			case 51:
			{
				obj = new Brick(float(j * 16 + 8), float(i * 16 + 8));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t - 1));
				//staticObject.push_back(obj);
				break;
			}
			//object magma
			case 107:
			{
				obj = new CMagma(float(j * 16 + 8), float(i * 16 + 8));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t - 1));
				//staticObject.push_back(obj);
				break;
			}
			//object gate
			case 46:case 48:
			{
				obj = new CGate(float(j * 16 + 8), float(i * 16 + 8));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t - 1));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t + 10));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t + 11));
				//staticObject.push_back(obj);
				break;
			}
			//object ladder
			case 8:
			{
				obj = new CLadder(float(j * 16 + 8), float(i * 16 + 8));
				obj->AddSprite(CSpriteManager::GetInstance()->Get(20000 + t - 1));
				//staticObject.push_back(obj);
				break;
			}
			//none object (background)
			default:
				break;
			}
			if (obj != NULL)
			{
				// Tinh chi so hang + cot de xac dinh grid
				// CT: chi so hang = Y / chieu cao grid
				// CT: chi so cot  = X / chieu rong grid
				int indexRow = obj->GetPosition().y / gridHeight;
				int indexCol = obj->GetPosition().x / gridWidth;

				grids[indexRow][indexCol]->AddObject(obj);
				//staticObject.push_back(obj);

			}
		}
	main->AddAnimation(500);
	main->SetPosition(100, 100);
	main->SetState(IDLE);
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
	jumper2->SetState(JUMPER2_STATE_WALKING_LEFT);
	objects.push_back(insect);
	objects.push_back(orb);
	objects.push_back(jumper2);*/
}


