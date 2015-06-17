#include "Scene.h"	


void Scene::initialize(const UINT &w, const UINT &h, const HWND &window, const HINSTANCE &inst)
{
	DirectX11Core::initialize(w, h, window, 1);

	t.initialize();
	s.initialize();

	terrain.initialize("res/terrain.aaa");

	box.initialize("res/crate.aaa");
	box.setWorldMatrix(XMMatrixTranspose(XMMatrixTranslation(-2.0f,0.5f,0.0f)));
	sphere.initialize("res/teapot.aaa");
	sphere.setWorldMatrix(XMMatrixTranspose(XMMatrixRotationX(-XM_PIDIV2) * XMMatrixTranslation(5.0f, 0.0f, 0.0f)));

	car.initialize("res/car.aaa");


	terrain.setWorldMatrix(XMMatrixTranspose(XMMatrixRotationX(XM_PIDIV2)));
	cbl.lightCol = XMFLOAT4(1, 1, 1, 1);
	cbl.lightPosW = XMFLOAT3(0, 0, -1);
	light = DirectX11Core::createConstantBuffer(sizeof(CBLight), 1, 0, &cbl);

	c = new FirstPersonCamera;
	c->initialize(DirectX11Core::mWindowWidth, DirectX11Core::mWindowHeight);

	Input::initialize(window, inst);
}
void Scene::render()
{
	t.begin();

	DirectX11Core::clearRenderTargetViews();

	terrain.draw();
	box.draw();
	sphere.draw();
	car.draw();

	static float f = 0.0f; f += 0.001f;

	Input::update();

	//cbl.lightPosW = XMFLOAT3(-2 * sin(f), 2 * cos(f), -1);
	
	cbl.lightPosW = XMFLOAT3(XMVectorGetX(c->getPos()), XMVectorGetY(c->getPos()), XMVectorGetZ(c->getPos()));

	DirectX11Core::mDeviceContext->UpdateSubresource(light, 0, 0, &cbl, 0, 0);
	DirectX11Core::mDeviceContext->VSSetConstantBuffers(2, 1, &light);

	float lr = 0, bf = 0, dt = t.getDelta(), factor = 4.5f;
	if (Input::isKeyDown(DIK_W)) {
		bf = dt * factor;
	}
	else if (Input::isKeyDown(DIK_S)) {
		bf = -dt * factor;
	}
	if (Input::isKeyDown(DIK_A)) {
		lr = -dt * factor;
	}
	else if (Input::isKeyDown(DIK_D)) {
		lr = dt * factor;
	}
	static float yaw = 0, pitch = 0, mouseFactor = 1.2f;
	yaw += Input::getMouseX() * dt * mouseFactor;
	pitch += Input::getMouseY() * dt * mouseFactor;
	c->update(lr, bf, yaw, pitch);

	s.draw(c->getPos());

	DirectX11Core::endScene();
	t.end(1);

	FrameStats fs = t.getStats();
	std::wstring s = L"FPS: ";
	s.append(std::to_wstring((int)fs.fps));
	s.append(L" frametime: ");
	s.append(std::to_wstring(fs.msPerFrame));
	s.append(L" (ms)");
	SetWindowText(DirectX11Core::mWindow, s.c_str());
}
void Scene::destroy()
{
	delete c;
	Input::destroy();
	DirectX11Core::destroy();
}