#pragma once
#include "DirectX11Core.h"
#include "VertexStructures.h"

class Sky
{
public:
	~Sky() { safeRelease(DSLessEqual); safeRelease(RSCullNone); }
public:
	bool initialize();
	void draw(const XMVECTOR &camPos);
private:
	ID3D11Buffer *mVBuffer;
	ID3D11Buffer *mIBuffer;
	ID3D11Buffer *mCBuffer;
	ID3D11InputLayout *mInputLayout;
	ID3D11PixelShader *mPShader;
	ID3D11VertexShader *mVShader;
	ID3D11SamplerState *mSamplerState;
	ID3D11ShaderResourceView *mTextureSRV;


	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;


	CBufferWorld mCBData;
	UINT mVerticeCount, mIndiceCount, mStride, mOffset;
};