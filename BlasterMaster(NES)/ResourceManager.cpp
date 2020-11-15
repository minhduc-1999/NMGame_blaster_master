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
	sprites->Add(10001, texPlayerSophia, 109, -11, 109 + 26, -11 + 34);
	sprites->Add(10002, texPlayerSophia, 77, -11, 77 + 26, -11 + 34);
	sprites->Add(10003, texPlayerSophia, 44, -11, 44 + 26, -11 + 34);
	sprites->Add(10004, texPlayerSophia, 13, -11, 13 + 26, -11 + 34);
	//sophia run low
	sprites->Add(10101, texPlayerSophia, 246, 14, 246 + 26, 14 + 34);
	sprites->Add(10102, texPlayerSophia, 214, 14, 214 + 26, 14 + 34);
	sprites->Add(10103, texPlayerSophia, 182, 14, 182 + 26, 14 + 34);
	sprites->Add(10104, texPlayerSophia, 149, 14, 149 + 26, 14 + 34);
	//sophia jump up
	sprites->Add(10201, texPlayerSophia, 109, 117, 109 + 26, 117 + 34);
	sprites->Add(10202, texPlayerSophia, 77, 117, 77 + 26, 117 + 34);
	sprites->Add(10203, texPlayerSophia, 44, 117, 44 + 26, 117 + 34);
	sprites->Add(10204, texPlayerSophia, 10, 117, 10 + 26, 117 + 34);
	//sophia jump down
	sprites->Add(10301, texPlayerSophia, 109, 38, 109 + 26, 38 + 34);
	sprites->Add(10302, texPlayerSophia, 78, 38, 78 + 26, 38 + 34);
	sprites->Add(10303, texPlayerSophia, 44, 38, 44 + 26, 38 + 34);
	sprites->Add(10304, texPlayerSophia, 12, 38, 12 + 26, 38 + 34);
	//sophia turn run
	sprites->Add(10401, texPlayerSophia, 111, 76, 111 + 24, 76 + 34);
	sprites->Add(10402, texPlayerSophia, 146, 76, 146 + 24, 76 + 34);
	//sophia turn jump
	sprites->Add(10501, texPlayerSophia, 111, 175, 111 + 24, 175 + 34);
	sprites->Add(10502, texPlayerSophia, 146, 175, 146 + 24, 175 + 34);
	//sophia up
	sprites->Add(10601, texPlayerSophia, 208, 153, 208 + 34, 153 + 34);
	sprites->Add(10602, texPlayerSophia, 208, 203, 208 + 34, 203 + 34);
	sprites->Add(10603, texPlayerSophia, 208, 246, 208 + 34, 246 + 34);
	sprites->Add(10604, texPlayerSophia, 204, 283, 204 + 34, 283 + 34);
	//sophia up run high
	sprites->Add(10701, texPlayerSophia, 204, 283, 204 + 34, 283 + 34);
	sprites->Add(10702, texPlayerSophia, 147, 283, 147 + 34, 283 + 34);
	sprites->Add(10703, texPlayerSophia, 83, 283, 83 + 34, 283 + 34);
	sprites->Add(10704, texPlayerSophia, 5, 283, 5 + 34, 283 + 34);
	//sophia up run low
	sprites->Add(10801, texPlayerSophia, 203, 366, 203 + 34, 366 + 34);
	sprites->Add(10802, texPlayerSophia, 143, 366, 143 + 34, 366 + 34);
	sprites->Add(10803, texPlayerSophia, 84, 366, 84 + 34, 366 + 34);
	sprites->Add(10804, texPlayerSophia, 9, 366, 9 + 34, 366 + 34);
	//sophia up jump
	sprites->Add(10901, texPlayerSophia, 201, 414, 201 + 34, 414 + 37);
	sprites->Add(10902, texPlayerSophia, 139, 414, 139 + 34, 414 + 37);
	sprites->Add(10903, texPlayerSophia, 72, 414, 72 + 34, 414 + 37);
	sprites->Add(10904, texPlayerSophia, 9, 414, 9 + 34, 414 + 37);
	//sophia down
	sprites->Add(11001, texPlayerSophia, 204, 283, 204 + 34, 283 + 34);
	sprites->Add(11002, texPlayerSophia, 208, 246, 208 + 34, 246 + 34);
	sprites->Add(11003, texPlayerSophia, 208, 203, 208 + 34, 203 + 34);
	sprites->Add(11004, texPlayerSophia, 208, 153, 208 + 34, 153 + 34);

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
