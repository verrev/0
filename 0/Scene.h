#pragma once
#include "Renderable.h"
#include "Camera.h"
#include "Input.h"
#include "Sky.h"
#include "Timer.h"


struct CBLight
{
	XMFLOAT3 lightPosW;
	XMFLOAT4 lightCol;
	float pad;
};

class Scene
{
public:
	void initialize(const UINT &w, const UINT &h, const HWND &window, const HINSTANCE &inst);
	void render();
	void destroy();
private:
	Timer t;
	Sky s;
	CBLight cbl;
	Renderable terrain;
	Renderable box;
	Renderable sphere;
	Renderable car;
	ID3D11Buffer *light;
	Camera *c;
};