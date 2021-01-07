#pragma once
#include "CGate.h"
class SceneGate : public CGate
{
private:
	int desScene;
public:
	SceneGate(float x, float y, int sceneId, int sectionId, D3DXVECTOR2 tele, int width, int height);
	int GetDesScene() { return desScene; }
};