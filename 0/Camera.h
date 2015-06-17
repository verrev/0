#pragma once
#include "DirectX11Core.h"

struct CBCamera
{
	XMMATRIX viewProj;
};

class Camera
{
public:
	virtual void initialize(const UINT &width, const UINT &height) = 0;
	virtual void update(const float &lr, const float &bf, const float &yaw, const float &pitch) = 0;
	inline virtual XMVECTOR getPos() = 0;
};

class FirstPersonCamera : public Camera
{
public:
	void initialize(const UINT &width, const UINT &height) override;
	void update(const float &lr, const float &bf, const float &yaw, const float &pitch) override;
	inline XMVECTOR getPos() override
	{
		return mPosition;
	}
private:
	ID3D11Buffer *mCBuffer;
	CBCamera mCBData;
	XMFLOAT4X4 mView, mProjection;
	XMVECTOR mPosition, mTarget, mUp;
	XMVECTOR mDefFwd, mDefRight;
	XMVECTOR mRight, mForward;
	float mMoveLR, mMoveBF;
	float mYaw, mPitch;
};