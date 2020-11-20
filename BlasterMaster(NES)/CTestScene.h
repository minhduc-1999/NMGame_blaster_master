#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Sophia.h"
#include "MiniJason.h"
#include "Orb.h"
#include "Jumper2.h"
#include "Insect.h"
#include "StaticGameObject.h"
#include "Grid.h"
#include "Skull.h"
#include "Teleporter.h"
#include "Mine.h"

#define PLAYER_SOPHIA 0
#define PLAYER_JASON  1

class CTestScene : public CScene
{
protected:
	CDynamicGameObject* mainPlayer;
	Sophia* sophia;	// A play scene has to have player, right?
	MiniJason* miniJason;
	int playerType = PLAYER_SOPHIA;

	float gridWidth;
	float gridHeight;
	int gridCol, gridRow;
	float mapX, mapY, mapWidth, mapHeight;

	vector<CDynamicGameObject*> dynamicObj;
	vector<CStaticGameObject*> staticObj;

	unordered_map<int, LPGRID> grids;
	unordered_map<int, unordered_map<int, int>> map;

	void GetMapInfo(string path);

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_DYNAMIC_OBJECTS(string line);
	void _ParseSection_STATIC_OBJECTS(string line);

public:
	CTestScene();
	CTestScene(int id, string filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CDynamicGameObject* GetPlayer() { return mainPlayer; }
	MiniJason* GetPlayerJason() { return miniJason; }
	Sophia* GetPlayerSophia() { return sophia; }
	void SetPlayer(CDynamicGameObject* player) { mainPlayer = player; }
	int GetPlayerType() { return playerType; }
	void ChangePlayerType() 
	{
		if (playerType == PLAYER_SOPHIA)
		{
			playerType = PLAYER_JASON;
		}
		else
		{
			playerType = PLAYER_SOPHIA;
		}
	}
	//MiniJason* GetPlayerJason() { return miniJason; }
	D3DXVECTOR3 GetSceneMapPos() { return D3DXVECTOR3{ mapX, mapY, 0 }; }
	D3DXVECTOR3 GetSceneMapDimension() { return D3DXVECTOR3{ mapWidth, mapHeight, 0 }; }
	D3DXVECTOR2 GetBoundGrid(Rect bound);

	//friend class CTestSceneKeyHandler;
};

class CTestSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CTestSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

