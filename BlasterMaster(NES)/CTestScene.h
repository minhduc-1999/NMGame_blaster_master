#pragma once
#include "Game.h"
#include "Scene.h"
#include "GameObject.h"
#include "Sophia.h"
#include "MiniJason.h"
#include "StaticGameObject.h"
#include "Section.h"
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

	unordered_map<int, LPSECTION> sections;
	int current_section;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_SECTION(string line);
	void _ParseSection_SETTINGS(string line);

public:
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
	void SwitchSection(int section_id);

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

