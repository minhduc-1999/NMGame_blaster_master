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


class CPlayScene: public CScene
{
protected: 
	Enemy1 *player;					// A play scene has to have player, right? 
	int gridWidth;
	int gridHeight;
	vector<LPGAMEOBJECT> objects;
	vector<LPSTATICOBJECT> staticObject;
	unordered_map<int, unordered_map<int, int>> map;
	void LoadMap(string path);

	/*void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);*/

	
public: 
	CPlayScene();
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	Enemy1 * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

