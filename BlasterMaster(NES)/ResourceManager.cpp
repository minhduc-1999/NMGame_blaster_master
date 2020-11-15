#include "ResourceManager.h"

ResourceManager* ResourceManager::__instance = NULL;

ResourceManager::ResourceManager()
{
}

void ResourceManager::LoadResource()
{
	textures->Add(ID_TEX_ENEMY, "enemy.png", D3DCOLOR_XRGB(0,0,255));
	//textures->Add(ID_TEX_BOX, "bbox.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_PLAYER_JASON, "Jason.png", D3DCOLOR_XRGB(0, 0, 255));
	textures->Add(ID_TEX_PLAYER_SOPHIA, "Sophia.png", D3DCOLOR_XRGB(0, 0, 255));
	textures->Add(ID_TEX_TILE, "area2-tile.png", D3DCOLOR_XRGB(156, 219, 239));
	//textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(156, 219, 239));

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);
	LPDIRECT3DTEXTURE9 texTile = textures->Get(ID_TEX_TILE);
	LPDIRECT3DTEXTURE9 texPlayerSophia = textures->Get(ID_TEX_PLAYER_SOPHIA);

	// readline => id, left, top, right, bottom 

	//sophia run high
	sprites->Add(10001, texPlayerSophia, 109, 5, 109 + 26, 5 + 18);
	sprites->Add(10002, texPlayerSophia, 77, 5, 77 + 26, 5 + 18);
	sprites->Add(10003, texPlayerSophia, 44, 5, 44 + 26, 5 + 18);
	sprites->Add(10004, texPlayerSophia, 13, 5, 13 + 26, 5 + 18);
	//sophia run low
	sprites->Add(10101, texPlayerSophia, 109, 30, 109 + 26, 30 + 18);
	sprites->Add(10102, texPlayerSophia, 77, 30, 77 + 26, 30 + 18);
	sprites->Add(10103, texPlayerSophia, 44, 30, 44 + 26, 30 + 18);
	sprites->Add(10104, texPlayerSophia, 13, 30, 13 + 26, 30 + 18);
	//sophia jump up
	sprites->Add(10201, texPlayerSophia, 109, 130, 109 + 26, 130 + 21);
	sprites->Add(10202, texPlayerSophia, 77, 130, 77 + 26, 130 + 21);
	sprites->Add(10203, texPlayerSophia, 44, 130, 44 + 26, 130 + 21);
	sprites->Add(10204, texPlayerSophia, 10, 130, 10 + 26, 130 + 21);
	//sophia jump down
	sprites->Add(10301, texPlayerSophia, 109, 56, 109 + 26, 56 + 16);
	sprites->Add(10302, texPlayerSophia, 78, 56, 78 + 26, 56 + 16);
	sprites->Add(10303, texPlayerSophia, 44, 56, 44 + 26, 56 + 16);
	sprites->Add(10304, texPlayerSophia, 12, 56, 12 + 26, 56 + 16);
	//sophia turn run
	sprites->Add(10401, texPlayerSophia, 111, 80, 111 + 24, 80 + 18);
	sprites->Add(10402, texPlayerSophia, 146, 80, 146 + 24, 80 + 18);
	//sophia turn jump
	sprites->Add(10501, texPlayerSophia, 111, 157, 111 + 24, 157 + 21);
	sprites->Add(10502, texPlayerSophia, 146, 157, 146 + 24, 157 + 21);
	//sophia up
	sprites->Add(10601, texPlayerSophia, 208, 187, 208 + 34, 187 + 26);
	sprites->Add(10602, texPlayerSophia, 208, 218, 208 + 34, 218 + 28);
	sprites->Add(10603, texPlayerSophia, 208, 250, 208 + 34, 250 + 30);
	sprites->Add(10604, texPlayerSophia, 204, 283, 204 + 34, 283 + 34);
	//sophia up run high
	sprites->Add(10701, texPlayerSophia, 204, 283, 204 + 34, 283 + 34);
	sprites->Add(10702, texPlayerSophia, 147, 283, 147 + 34, 283 + 34);
	sprites->Add(10703, texPlayerSophia, 83, 283, 83 + 34, 283 + 34);
	sprites->Add(10704, texPlayerSophia, 5, 283, 5 + 34, 283 + 34);
	//sophia up run low
	sprites->Add(10801, texPlayerSophia, 203, 367, 203 + 34, 367 + 33);
	sprites->Add(10802, texPlayerSophia, 143, 367, 143 + 34, 367 + 33);
	sprites->Add(10803, texPlayerSophia, 84, 367, 84 + 34, 367 + 33);
	sprites->Add(10804, texPlayerSophia, 9, 367, 9 + 34, 367 + 33);
	//sophia up jump
	sprites->Add(10901, texPlayerSophia, 201, 414, 201 + 34, 414 + 37);
	sprites->Add(10902, texPlayerSophia, 139, 414, 139 + 34, 414 + 37);
	sprites->Add(10903, texPlayerSophia, 72, 414, 72 + 34, 414 + 37);
	sprites->Add(10904, texPlayerSophia, 9, 414, 9 + 34, 414 + 37);
	//sophia down
	sprites->Add(11001, texPlayerSophia, 204, 283, 204 + 34, 283 + 34);
	sprites->Add(11002, texPlayerSophia, 208, 250, 208 + 34, 250 + 30);
	sprites->Add(11003, texPlayerSophia, 208, 218, 208 + 34, 218 + 28);
	sprites->Add(11004, texPlayerSophia, 208, 187, 208 + 34, 187 + 26);

	//load sprites of insect

	////insect left fly
	//sprites->Add(10001, texEnemy, 84, 530, 102, 548);
	//sprites->Add(10002, texEnemy, 104, 530, 122, 548);

	////load sprites of Orb

	//sprites->Add(10011, texEnemy, 78, 387, 96, 405);
	//sprites->Add(10012, texEnemy, 98, 387, 116, 405);
	//sprites->Add(10013, texEnemy, 118, 387, 136, 405);
	//sprites->Add(10014, texEnemy, 138, 387, 156, 405);
	//sprites->Add(10015, texEnemy, 158, 387, 176, 405);



	////load sprites of jumper 2
	//sprites->Add(10021, texEnemy, 69, 500, 69 + 17, 500 + 26);
	//sprites->Add(10022, texEnemy, 87, 499, 87 + 17, 499 + 26);
	//sprites->Add(10023, texEnemy, 105, 500, 105 + 17, 500 + 26);

	//load tile
	for (int i = 0; i < 13; i++)
		for (int j = 0; j < 11; j++)
		{
			int left = 1 + j * 20;
			int top = 1 + i * 20;
			sprites->Add(20000 + i * 11 + j, texTile, left, top, left + 16, top + 16);
		}
	LPANIMATION ani;

	//add anim SOPHIA RUN
	ani = new CAnimation(25);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	ani->Add(10004);
	animations->Add(100, ani);

	ani = new CAnimation(25);
	ani->Add(10101);
	ani->Add(10102);
	ani->Add(10103);
	ani->Add(10104);
	animations->Add(101, ani);

	ani = new CAnimation(25);
	ani->Add(10201);
	ani->Add(10202);
	ani->Add(10203);
	ani->Add(10204);
	animations->Add(102, ani);

	ani = new CAnimation(200);
	ani->Add(10301);
	ani->Add(10302);
	ani->Add(10303);
	ani->Add(10304);
	animations->Add(103, ani);

	ani = new CAnimation(400);
	ani->Add(10401);
	ani->Add(10402);
	animations->Add(104, ani);

	ani = new CAnimation(400);
	ani->Add(10501);
	ani->Add(10502);
	animations->Add(105, ani);

	ani = new CAnimation(50);
	ani->Add(10601);
	ani->Add(10602);
	ani->Add(10603);
	ani->Add(10604);
	animations->Add(106, ani);

	ani = new CAnimation(25);
	ani->Add(10701);
	ani->Add(10702);
	ani->Add(10703);
	ani->Add(10704);
	animations->Add(107, ani);

	ani = new CAnimation(25);
	ani->Add(10801);
	ani->Add(10802);
	ani->Add(10803);
	ani->Add(10804);
	animations->Add(108, ani);

	ani = new CAnimation(25);
	ani->Add(10901);
	ani->Add(10902);
	ani->Add(10903);
	ani->Add(10904);
	animations->Add(109, ani);

	ani = new CAnimation(50);
	ani->Add(11001);
	ani->Add(11002);
	ani->Add(11003);
	ani->Add(11004);
	animations->Add(110, ani);

	////add anim Orb
	////run left
	//ani = new CAnimation(100);
	//ani->Add(10011);
	//animations->Add(510, ani);
	////turn right
	//ani = new CAnimation(100);
	//ani->Add(10011);
	//ani->Add(10012);
	//ani->Add(10013);
	//ani->Add(10014);
	//ani->Add(10015);
	//animations->Add(511, ani);

	////add anim jumper 2
	//ani = new CAnimation(250);
	//ani->Add(10021);
	//ani->Add(10022);
	//ani->Add(10023);
	//animations->Add(520, ani);
}

ResourceManager* ResourceManager::GetInstance()
{
	if (__instance == NULL)
		__instance = new ResourceManager();
	return __instance;
}
