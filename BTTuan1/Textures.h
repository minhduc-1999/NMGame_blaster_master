#pragma once
#include <d3dx9.h>
#include <unordered_map>
#include "Sprites.h"
using namespace std;

class CTextureManager
{
	static CTextureManager* __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;
public:
	CTextureManager();
	void Add(int id, LPCSTR filePath, D3DCOLOR transColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int id);

	static CTextureManager* GetInstance();
};

