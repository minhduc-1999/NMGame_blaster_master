#pragma once
#include "Textures.h"

#define ID_TEX_ENEMY 0
#define ID_TEX_MISC 20
#define ID_TEX_BOX	30
#define ID_TEX_TILE 40

class ResourceManager
{
private:
	static ResourceManager* __instance;
	CTextureManager* textures = CTextureManager::GetInstance();
	CSpriteManager* sprites = CSpriteManager::GetInstance();
	CAnimationManager* animations = CAnimationManager::GetInstance();
public:
	ResourceManager();
	static ResourceManager* GetInstance();
	void LoadResource();
};

