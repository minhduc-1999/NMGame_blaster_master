#pragma once
#include "Game.h"
#include "Scene.h"
#include "GameObject.h"
#include "Sophia.h"
#include "MiniJason.h"
#include "StaticGameObject.h"
#include "Section.h"
#include "Mine.h"
#include "Boss.h"
#include "SectionTransition.h"
#include "HPBar.h"
#include "Sound.h"

#define TEXTURE_BACKGROUND 40
#define TEXTURE_FOREGROUND 50
#define PLAYER_SOPHIA 0
#define PLAYER_JASON  1

class CTestScene : public CScene
{
protected:
	CDynamicGameObject* mainPlayer;
	Sophia* sophia;	// A play scene has to have player, right?
	MiniJason* miniJason;
	int playerType;
	LPSECTRANSITION transition;
	HPBar* hpBar;
	unordered_map<int, LPSECTION> sections;
	int current_section;
	bool isSwitchingSection;
public:
	CTestScene(int id, string filePath, int type, D3DXVECTOR3 bg);

	virtual void Load(D3DXVECTOR2 tlPos);
	virtual int Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void SwitchSection(int section_id, D3DXVECTOR2 telePos);
	void SetHPBarState(int state) { this->hpBar->SetState(state); }

	void SetCurrentSection(int cur) { current_section = cur; }
	int GetCurrentSection() { return current_section; }
	LPSECTION GetCurSection() { return sections.at(current_section); }

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_SECTION(string line);
	void _ParseSection_SOUND(string line);

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

	void addMiniJason()
	{
		sections[current_section]->AddMiniJason();
		mainPlayer = (MiniJason*)sections[current_section]->GetPlayer();
	}

	void deleteMiniJason()
	{
		sections[current_section]->DeleteMiniJason();
		mainPlayer = (Sophia*)sections[current_section]->GetPlayer();
	}

	//MiniJason* GetPlayerJason() { return miniJason; }

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

