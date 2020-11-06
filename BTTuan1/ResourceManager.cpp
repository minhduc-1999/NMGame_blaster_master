#include "ResourceManager.h"

ResourceManager* ResourceManager::__instance = NULL;

ResourceManager::ResourceManager()
{
}

void ResourceManager::LoadResource()
{
	textures->Add(ID_TEX_ENEMY, "enemy.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_BOX, "bbox.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_TILE, "area2-tile.png", D3DCOLOR_XRGB(156, 219, 239));
	//textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(156, 219, 239));

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);
	LPDIRECT3DTEXTURE9 texTile = textures->Get(ID_TEX_TILE);

	// readline => id, left, top, right, bottom 

	//load sprites of insect

	//insect left fly
	sprites->Add(10001, texEnemy, 84, 530, 102, 548);
	sprites->Add(10002, texEnemy, 104, 530, 122, 548);

	//load sprites of Orb

	sprites->Add(10011, texEnemy, 78, 387, 96, 405);
	sprites->Add(10012, texEnemy, 98, 387, 116, 405);
	sprites->Add(10013, texEnemy, 118, 387, 136, 405);
	sprites->Add(10014, texEnemy, 138, 387, 156, 405);
	sprites->Add(10015, texEnemy, 158, 387, 176, 405);



	//load sprites of jumper 2
	sprites->Add(10021, texEnemy, 69, 500, 69 + 17, 500 + 26);
	sprites->Add(10022, texEnemy, 87, 499, 87 + 17, 499 + 26);
	sprites->Add(10023, texEnemy, 105, 500, 105 + 17, 500 + 26);

	//load tile
	for (int i = 0; i < 13; i++)
		for (int j = 0; j < 11; j++)
		{
			int left = 1 + j * 20;
			int top = 1 + i * 20;
			sprites->Add(20000 + i * 11 + j, texTile, left, top, left + 16, top + 16);
		}
	LPANIMATION ani;

	//add anim Insect
	ani = new CAnimation(200);
	ani->Add(10001);
	ani->Add(10002);
	animations->Add(500, ani);

	//add anim Orb
	//run left
	ani = new CAnimation(100);
	ani->Add(10011);
	animations->Add(510, ani);
	//turn right
	ani = new CAnimation(100);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	ani->Add(10014);
	ani->Add(10015);
	animations->Add(511, ani);

	//add anim jumper 2
	ani = new CAnimation(250);
	ani->Add(10021);
	ani->Add(10022);
	ani->Add(10023);
	animations->Add(520, ani);
}

ResourceManager* ResourceManager::GetInstance()
{
	if (__instance == NULL)
		__instance = new ResourceManager();
	return __instance;
}
