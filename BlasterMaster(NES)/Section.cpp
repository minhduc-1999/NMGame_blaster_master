#include "Section.h"
#include <fstream>
#include "Brick.h"
#include "CMagma.h"
#include "Sophia.h"
#include "CGate.h"
#include "Mine.h"
#include "Skull.h"
#include "Teleporter.h"
#include "Floater2.h"
#include "Insect.h"
#include "Jumper2.h"
#include "Orb.h"
#include "Cannon.h"
#include "Eyeball.h"
#include "HPBar.h"
#include "Boss.h"
#include "Worm.h"
#include "Dome.h"
#include "Jason.h"
#include "MiniJason.h"
#include "CLadder.h"
#include "SceneGate.h"
#include "Rock.h"
using namespace std;

#pragma region SECTION CONFIG
#define SECTION_SECTION_UNKNOWN -1
#define SECTION_SECTION_MAP 1
#define SECTION_SECTION_GRID 2
#define SECTION_SECTION_STATIC_OBJECTS	3
#define SECTION_SECTION_DYNAMIC_OBJECTS	4
#define SECTION_SECTION_DEFAULT			5

#pragma endregion

#pragma region OBJECT TYPE
#define OBJECT_TYPE_SOPHIA		1
#define OBJECT_TYPE_MINI_JASON	2
#define OBJECT_TYPE_JASON		3
#define OBJECT_TYPE_BOSS		40
#define OBJECT_TYPE_WORM		4
#define OBJECT_TYPE_DOME		5
#define OBJECT_TYPE_FLOATER2	6
#define OBJECT_TYPE_INSECT		7
#define OBJECT_TYPE_JUMPER2		8
#define OBJECT_TYPE_ORB			9
#define OBJECT_TYPE_SKULL		10
#define OBJECT_TYPE_MINE		11
#define OBJECT_TYPE_TELEPORTER	12
#define OBJECT_TYPE_CANNON		13
#define OBJECT_TYPE_EYEBALL		14
#define OBJECT_TYPE_BRICK		15
#define OBJECT_TYPE_ARROW		16
#define OBJECT_TYPE_GATE		17
#define OBJECT_TYPE_LADDER		18
#define OBJECT_TYPE_MAGMA		19
#define OBJECT_TYPE_BULLET		20
#define OBJECT_TYPE_ITEM_HP		26
#define OBJECT_TYPE_ROCK		60
#pragma endregion

void Section::AddMiniJason()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_setSOPHIA = animation_sets->Get(OBJECT_TYPE_SOPHIA);
	LPANIMATION_SET ani_setMINIJASON = animation_sets->Get(OBJECT_TYPE_MINI_JASON);
	Sophia* sophia = new Sophia(mainPlayer->GetPosition().x, mainPlayer->GetPosition().y);
	if (mainPlayer->GetNX() == 1)
	{
		sophia->SetState(SOPHIA_STATE_IDLE_RIGHT);
	}
	else
	{
		sophia->SetState(SOPHIA_STATE_IDLE_LEFT);
	}
	sophia->SetAnimationSet(ani_setSOPHIA);
	sophia->SetTeam(0);
	sophia->SetType(OBJECT_TYPE_SOPHIA);
	vector<int> rs = GetBoundGrid(sophia->GetBound());
	for (int i = 0; i < rs.size(); i++)
	{
		grids[rs[i]]->AddDynamicObj(sophia);
	}
	MiniJason* newMiniJason = new MiniJason(mainPlayer->GetPosition().x, mainPlayer->GetPosition().y);
	newMiniJason->SetState(MINIJASON_STATE_IDLE_RIGHT);
	newMiniJason->SetTeam(0);
	newMiniJason->SetType(OBJECT_TYPE_MINI_JASON);
	newMiniJason->SetAnimationSet(ani_setMINIJASON);
	mainPlayer = (MiniJason*)newMiniJason;
}

void Section::DeleteMiniJason()
{
	vector<int> rs = GetBoundGrid(CGame::GetInstance()->GetCamBound());
	for (int i = 0; i < rs.size(); i++)
	{
		Sophia* temp = grids[rs[i]]->GetSophia();
		if (temp != NULL)
		{
			mainPlayer = temp;
		}
	}
}

void Section::AddDynamicObject(LPDYNAMICOBJECT obj)
{
	if (obj == NULL)
		return;
	vector<int> boundGrid = GetBoundGrid(obj->GetBound());
	for (int i = 0; i < boundGrid.size(); i++)
	{
		if (grids.find(boundGrid[i]) != grids.end())
		{
			grids[boundGrid[i]]->AddDynamicObj(obj);
		}
	}
}

void Section::_ParseSection_DYNAMIC_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int grid = atof(tokens[3].c_str());
	int ani_set_id = atoi(tokens[4].c_str());

	int dome_state = 0;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	CDynamicGameObject* obj = NULL;

	if (object_type == OBJECT_TYPE_BOSS)
	{
		int	bossHand_ani_set_id;
		int bossArm_ani_set_id;
		Boss* boss;
		vector<LPDYNAMICOBJECT> hands;
		bossHand_ani_set_id = atoi(tokens[5].c_str());
		bossArm_ani_set_id = atoi(tokens[6].c_str());
		obj = new Boss(x, y, bossHand_ani_set_id, bossArm_ani_set_id);
		boss = (Boss*)obj;
		hands = boss->CreateHands();
		obj->SetTeam(1);
		obj->SetType(object_type);
		obj->SetAnimationSet(ani_set);
		obj->SetState(BOSS_STATE_FLYING);
		grids[grid]->AddDynamicObj(obj);
		for (int i = 0; i < hands.size(); i++)
		{
			grids[grid]->AddDynamicObj(hands[i]);
		}
		return;
	}
	else
	{
		switch (object_type)
		{
			//dynamic obj
		//case OBJECT_TYPE_SOPHIA:
		//{
		//	if (mainPlayer != NULL)
		//	{
		//		//D3DXVECTOR2 pos = mainPlayer->GetPosition();
		//		//DebugOut("[Pos player trans before load]\tx: %f, y: %f\n", pos.x, pos.y);
		//		DebugOut("[ERROR] main object was created before!\n");
		//		return;
		//	}
		//	obj = new Sophia(x, y);
		//	mainPlayer = (Sophia*)obj;
		//	obj->SetAnimationSet(ani_set);
		//	obj->SetTeam(0);
		//	obj->SetType(object_type);
		//	DebugOut("[INFO] Player object created!\n");
		//	return;
		//	//DebugOut("[PLAYER POSITION]\t%f\t%f\n", x, y);
		//	break;
		//}
		//case OBJECT_TYPE_JASON:
		//	if (mainPlayer != NULL)
		//	{
		//		//D3DXVECTOR2 pos = mainPlayer->GetPosition();
		//		//DebugOut("[Pos player trans before load]\tx: %f, y: %f\n", pos.x, pos.y);
		//		DebugOut("[ERROR] main object was created before!\n");
		//		return;
		//	}
		//	obj = new Jason(x, y);
		//	mainPlayer = (Jason*)obj;
		//	obj->SetAnimationSet(ani_set);
		//	obj->SetTeam(0);
		//	obj->SetType(object_type);
		//	DebugOut("[INFO] Player object created!\n");
		//	return;
		//	break;
		case OBJECT_TYPE_FLOATER2:
			obj = new	Floater2(x, y);
			obj->SetState(FLOATER2_STATE_FLYING_LEFT);
			break;
		case OBJECT_TYPE_WORM:
			obj = new Worm(x, y);
			obj->SetState(WORM_STATE_WALKING_LEFT);
			break;
		case OBJECT_TYPE_DOME:
			obj = new Dome(x, y);
			dome_state = atoi(tokens[5].c_str());
			obj->SetState(dome_state);
			break;
		case OBJECT_TYPE_INSECT:
			obj = new Insect(x, y);
			obj->SetState(INSECT_STATE_ALIVE);
			break;
		case OBJECT_TYPE_JUMPER2:
			obj = new Jumper2(x, y);
			obj->SetState(JUMPER2_STATE_WALKING_RIGHT);
			break;
		case OBJECT_TYPE_ORB:
			obj = new Orb(x, y);
			obj->SetState(ORB_STATE_ROLLING_LEFT);
			break;
		case OBJECT_TYPE_SKULL:
			obj = new Skull(x, y);
			obj->SetState(SKULL_STATE_FLYING_LEFT);
			break;
		case OBJECT_TYPE_MINE:
			obj = new Mine(x, y);
			obj->SetState(MINE_STATE_ONGROUND);
			break;
		case OBJECT_TYPE_TELEPORTER:
			obj = new Teleporter(x, y);
			obj->SetState(TELEPORTER_STATE_GRAY);
			break;
		case OBJECT_TYPE_CANNON:
			obj = new Cannon(x, y);
			obj->SetState(CANNON_STATE_ALIVE);
			break;
		case OBJECT_TYPE_EYEBALL:
			obj = new Eyeball(x, y);
			obj->SetState(EYEBALL_STATE_ALIVE);
			break;
		case OBJECT_TYPE_ROCK:
			obj = new Rock(x, y);
			break;
		default:
			DebugOut("[ERROR] Invalid object type: %d\n", object_type);
			return;
		}
	}

	// General object setup
	//obj->SetPosition(x, y);
	obj->SetTeam(1);
	obj->SetType(object_type);
	obj->SetAnimationSet(ani_set);
	grids[grid]->AddDynamicObj(obj);
}

void Section::_ParseSection_STATIC_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines - an object set must have at least id, x, y, gridid, sprite id

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int grid = atoi(tokens[3].c_str());
	CStaticGameObject* obj = NULL;
	switch (object_type)
	{
		//static obj
	case OBJECT_TYPE_BRICK:
		obj = new Brick(x, y);
		//obj->SetType(object_type);
		break;
	case OBJECT_TYPE_LADDER:
		obj = new CLadder(x, y);
		obj->SetTeam(0);
		//obj->SetType(object_type);
		break;
	case 80:
	{
		int scene = atoi(tokens[tokens.size() - 4].c_str());
		int section = atoi(tokens[tokens.size() - 3].c_str());
		float telex = stof(tokens[tokens.size() - 2].c_str());
		float teley = stof(tokens[tokens.size() - 1].c_str());
		int size_w = atoi(tokens[tokens.size() - 6].c_str());
		int size_h = atoi(tokens[tokens.size() - 5].c_str());
		D3DXVECTOR2 telePos = D3DXVECTOR2(telex, teley);
		obj = new SceneGate(x, y, scene, section, telePos, size_w, size_h);
		obj->SetType(object_type);
		for (int i = 4; i <= tokens.size() - 7; i++)
		{
			int spriteID = atoi(tokens[i].c_str());
			obj->AddSprite(CSpriteManager::GetInstance()->Get(spriteID));
		}
		obj->SetTeam(0);
		grids[grid]->AddStaticObj(obj);
		return;
		break;
	}
	case OBJECT_TYPE_GATE:
	{
		int section = atoi(tokens[tokens.size() - 3].c_str());
		float telex = stof(tokens[tokens.size() - 2].c_str());
		float teley = stof(tokens[tokens.size() - 1].c_str());
		int size_w = atoi(tokens[tokens.size() - 5].c_str());
		int size_h = atoi(tokens[tokens.size() - 4].c_str());
		D3DXVECTOR2 telePos = D3DXVECTOR2(telex, teley);
		obj = new CGate(x, y, section, telePos, size_w, size_h);
		obj->SetType(object_type);
		obj->SetTeam(0);
		for (int i = 4; i <= tokens.size() - 6; i++)
		{
			int spriteID = atoi(tokens[i].c_str());
			obj->AddSprite(CSpriteManager::GetInstance()->Get(spriteID));
		}
		grids[grid]->AddStaticObj(obj);
		return;
		break;
	}
	case OBJECT_TYPE_MAGMA:case OBJECT_TYPE_ARROW:
		obj = new CMagma(x, y);
		break;
	default:
		DebugOut("[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	int spriteID = atoi(tokens[4].c_str());
	obj->SetType(object_type);
	obj->AddSprite(CSpriteManager::GetInstance()->Get(spriteID));
	grids[grid]->AddStaticObj(obj);
	//DebugOut("[STATIC OBJ POSITION]\t%d\t%f\t%f\n", object_type, x, y); 
}

void Section::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() != 4) return; // skip invalid lines - map must have 4 value

	mapX = atof(tokens[0].c_str());
	mapY = atof(tokens[1].c_str());
	mapWidth = atof(tokens[2].c_str());
	mapHeight = atof(tokens[3].c_str());
	DebugOut("[INFO] Done loading section map info!\n");
	//DebugOut("[MAP POSITION]\t%f\t%f\n", mapX, mapY);
}

void Section::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() != 2) return; // skip invalid lines - grid must have 2 value

	gridWidth = atof(tokens[0].c_str());
	gridHeight = atof(tokens[1].c_str());
	gridCol = mapWidth / gridWidth;
	gridRow = mapHeight / gridHeight;

	for (int i = 0; i < gridRow; i++)
		for (int j = 0; j < gridCol; j++)
		{
			int index = (i % gridRow) * gridCol + j % gridCol;
			Rect bound;
			bound.top = i * gridHeight + mapY;
			bound.left = j * gridWidth + mapX;
			bound.right = bound.left + gridWidth - 1;
			bound.bottom = bound.top + gridHeight - 1;
			LPGRID temp = new Grid(index, bound);
			grids.emplace(index, temp);
		}
	DebugOut("[INFO] Done loading section grid info!\n");
}

void Section::_ParseSection_DEFAULT(string line)
{
 	vector<string> tokens = split(line);

	if (tokens.size() != 2) return; // skip invalid lines - map must have 4 value
	defaultPos = D3DXVECTOR2(atof(tokens[0].c_str()), atof(tokens[1].c_str()));
	DebugOut("[INFO] Done loading default info!\n");
}

void Section::Load()
{
	DebugOut("[INFO] Start loading SECTION resources from : %s \n", secFilePath);

	ifstream f;
	f.open(secFilePath);
	string line;
	// current resource section flag
	int section = SECTION_SECTION_UNKNOWN;

	while (getline(f, line))
	{
		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[MAP]") { section = SECTION_SECTION_MAP; continue; }
		if (line == "[GRID]") { section = SECTION_SECTION_GRID; continue; }
		if (line == "[STATIC_OBJECTS]") {
			section = SECTION_SECTION_STATIC_OBJECTS; continue;
		}
		if (line == "[DYNAMIC_OBJECTS]") {
			section = SECTION_SECTION_DYNAMIC_OBJECTS; continue;
		}
		if (line == "[DEFAULT]") {
			section = SECTION_SECTION_DEFAULT; continue;
		}
		if (line[0] == '[') { section = SECTION_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SECTION_SECTION_DEFAULT: _ParseSection_DEFAULT(line); break;
		case SECTION_SECTION_GRID: _ParseSection_GRID(line); break;
		case SECTION_SECTION_MAP: _ParseSection_MAP(line); break;
		case SECTION_SECTION_DYNAMIC_OBJECTS: _ParseSection_DYNAMIC_OBJECTS(line); break;
		case SECTION_SECTION_STATIC_OBJECTS: _ParseSection_STATIC_OBJECTS(line); break;
		}
	}

	f.close();

	//CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	DebugOut("[INFO] Done loading SECTION resources %s\n", secFilePath);
}

int Section::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjs;
	Rect camPos = CGame::GetInstance()->GetCamBound();
	vector<int> camBoundGrid = GetBoundGrid(camPos);
	for (int i = 0; i < camBoundGrid.size(); i++)
	{
		if (grids.find(camBoundGrid[i]) != grids.end())
		{
			vector<LPGAMEOBJECT>* temp = grids.at(camBoundGrid[i])->GetcoObjectList();
			coObjs.insert(coObjs.end(), temp->begin(), temp->end());
		}
	}
	vector<LPDYNAMICOBJECT>* changeGridObjs;
	for (int i = 0; i < camBoundGrid.size(); i++)
	{
		if (grids.find(camBoundGrid[i]) != grids.end())
		{
			changeGridObjs = grids.at(camBoundGrid[i])->Update(dt, &coObjs, mainPlayer->GetPosition().x, mainPlayer->GetPosition().y);
			for (int j = 0; j < changeGridObjs->size(); j++)
			{
				LPDYNAMICOBJECT obj = changeGridObjs->at(j);
				vector<int> objBoundGrids;
				objBoundGrids = this->GetBoundGrid(obj->GetBound());
				for (int k = 0; k < objBoundGrids.size(); k++)
				{
					int index = objBoundGrids[k];
					grids[index]->AddDynamicObj(obj);
				}
			}
			delete changeGridObjs;
		}
	}
	return mainPlayer->Update(dt, &coObjs);
}

void Section::Render()
{
	//Kiem tra xem camera chiem grid nao
	Rect camPos = CGame::GetInstance()->GetCamBound();
	vector<int> grid = GetBoundGrid(camPos);
	for (int i = 0; i < grid.size(); i++)
	{
		if (grids.find(grid[i]) != grids.end())
		{
			grids.at(grid[i])->Render();
		}
	}
	//render main
	mainPlayer->Render();
}

void Section::Unload()
{
	/*unordered_map<int, LPGRID>::iterator temp = grids.begin();
	while (temp != grids.end())
	{
		temp->second->Clear();
		delete temp->second;
		temp = grids.erase(grids.begin());
	}*/
	for (int i = 0; i < grids.size(); i++)
	{
		LPGRID temp = grids[i];
		temp->Clear();
		delete temp;
	}
	grids.clear();
}

vector<int> Section::GetBoundGrid(Rect bound)
{
	vector<int> result;
	int startRow = (bound.top - mapY) / gridHeight;
	int startCol = (bound.left - mapX) / gridWidth;
	int endRow = (bound.bottom - mapY) / gridHeight;
	int endCol = (bound.right - mapX) / gridWidth;
	int maxIndex = gridRow * gridCol - 1;
	for (int i = startRow; i <= endRow; i++)
		for (int j = startCol; j <= endCol; j++)
		{
			int temp = (i % gridRow) * gridCol + j % gridCol;
			if (temp >= 0 && temp <= maxIndex)
				result.push_back(temp);
		}
	return result;
}
