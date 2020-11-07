#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Enemy1.h"
#include "Orb.h"
#include "Jumper2.h"
#include "Insect.h"
#include "StaticGameObject.h"
#include "Grid.h"


class CTestScene : public CScene
{
protected:
	Enemy1* player;	// A play scene has to have player, right?

	float gridWidth;
	float gridHeight;
	int gridCol, gridRow;
	float mapX, mapY, mapWidth, mapHeight;
	unordered_map<int, unordered_map<int, LPGRID>> grids;

	//vector<LPSTATICOBJECT> staticObject;
	unordered_map<int, unordered_map<int, int>> map;

	void GetMapInfo(string path);

	/*void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);*/


public:
	CTestScene();
	CTestScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	Enemy1* GetPlayer() { return player; }
	D3DXVECTOR3 GetSceneMapPos() { return D3DXVECTOR3{ mapX, mapY, 0 }; }
	D3DXVECTOR3 GetSceneMapDimension() { return D3DXVECTOR3{ mapWidth, mapHeight, 0 }; }
	RECT GetViewGrid(RECT camPos);
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

