#pragma once
#include "DirectX11Core.h"
#include "VertexStructures.h"

class Renderable
{
public:
	bool initialize(const std::string &modelPath);
	void draw();
	inline std::string getFilePath() const
	{
		return mFilePath;
	}
	inline void setWorldMatrix(const XMMATRIX &w)
	{
		mCBData.world = w;
	}
private:
	ID3D11Buffer *mVBuffer;
	ID3D11Buffer *mIBuffer;
	ID3D11Buffer *mCBuffer;
	ID3D11InputLayout *mInputLayout;
	ID3D11PixelShader *mPShader;
	ID3D11VertexShader *mVShader;
	ID3D11SamplerState *mSamplerState;
	ID3D11ShaderResourceView *mTextureSRV;
	CBufferWorld mCBData;
	UINT mVerticeCount, mIndiceCount, mStride, mOffset;
private:
	std::string mFilePath;
private:
	RenderableHeader getHeader(std::ifstream &file);
	void getIEDfromHeader(const RenderableHeader &h, D3D11_INPUT_ELEMENT_DESC *ied,UINT &inputLayoutSize, UINT &elementCount);
};