#include "SceneGate.h"

SceneGate::SceneGate(float x, float y, int sceneId, int sectionId, D3DXVECTOR2 tele, int width, int height) 
	: CGate(x, y, sectionId, tele, width, height)
{
	this->desScene = sceneId;
}
