#include "Renderable.h"
#include "Fileutils.h"

RenderableHeader Renderable::getHeader(std::ifstream &file)
{
	RenderableHeader header;
	file.read((char*)&header, sizeof(RenderableHeader));
	return header;
}
void Renderable::getIEDfromHeader(const RenderableHeader &h, D3D11_INPUT_ELEMENT_DESC *ied, UINT &inputLayoutSize, UINT &elementCount)
{
	UINT index = 0, size = 0;

	if (h.vsInputLayout & VERTEX_ELEMENT::POSITION) {
		ied[index].SemanticName = "POSITION";
		ied[index].SemanticIndex = 0;
		ied[index].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		ied[index].InputSlot = 0;
		ied[index].AlignedByteOffset = size;
		ied[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[index++].InstanceDataStepRate = 0;
		size += sizeof(XMFLOAT3);
	}
	if (h.vsInputLayout & VERTEX_ELEMENT::NORMAL) {
		ied[index].SemanticName = "NORMAL";
		ied[index].SemanticIndex = 0;
		ied[index].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		ied[index].InputSlot = 0;
		ied[index].AlignedByteOffset = size;
		ied[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[index++].InstanceDataStepRate = 0;
		size += sizeof(XMFLOAT3);
	}
	if (h.vsInputLayout & VERTEX_ELEMENT::TEXCOORD) {
		ied[index].SemanticName = "TEXCOORD";
		ied[index].SemanticIndex = 0;
		ied[index].Format = DXGI_FORMAT_R32G32_FLOAT;
		ied[index].InputSlot = 0;
		ied[index].AlignedByteOffset = size;
		ied[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[index++].InstanceDataStepRate = 0;
		size += sizeof(XMFLOAT2);
	}
	
	inputLayoutSize = size;
	elementCount = index;
}
bool Renderable::initialize(const std::string &modelPath)
{
	mFilePath = modelPath;
	mSamplerState = nullptr;
	mTextureSRV = nullptr;

	std::ifstream file;
	if (!Fileutils::readFile(mFilePath, file)) {
		LOG("error loading renderable from file");
		return 0;
	}

	RenderableHeader header = getHeader(file);
	mVerticeCount = header.verticeCount;
	mIndiceCount = header.indiceCount;

	D3D11_INPUT_ELEMENT_DESC ied[MAX_ELEMENT_COUNT];
	UINT elementCount = 0;
	getIEDfromHeader(header, ied, mStride, elementCount);

	std::vector<Vertex1P> v1P;
	std::vector<Vertex1P1N1UV> v1P1N1UV;
	void *v = nullptr;

	if (header.vsInputLayout & VERTEX_ELEMENT::POSITION &&
		header.vsInputLayout & VERTEX_ELEMENT::TEXCOORD &&
		header.vsInputLayout & VERTEX_ELEMENT::NORMAL) {
		v1P1N1UV.resize(mVerticeCount);
		v = &v1P1N1UV[0];
	}
	else {
		v1P.resize(mVerticeCount);
		v = &v1P[0];
	}

	file.read((char*)v, mVerticeCount * mStride);
	mVBuffer = DirectX11Core::createVertexBuffer(mStride * mVerticeCount, 0, 0, v);

	std::vector<UINT> i;
	i.resize(mIndiceCount);
	file.read((char*)&i[0], mIndiceCount * sizeof(UINT));
	mIBuffer = DirectX11Core::createIndexBuffer(sizeof(UINT) * mIndiceCount, 0, &i[0]);

	
	ID3DBlob *vsByteCode = DirectX11Core::readShaderFromFile(header.vshaderPath);
	mInputLayout = DirectX11Core::createInputLayout(vsByteCode, ied, elementCount);
	mVShader = DirectX11Core::createVertexShader(vsByteCode);

	ID3DBlob *psByteCode = DirectX11Core::readShaderFromFile(header.pshaderPath);
	mPShader = DirectX11Core::createPixelShader(psByteCode);
	
	file.read((char*)&mCBData.world, sizeof(XMMATRIX));
	mCBData.world = XMMatrixTranspose(mCBData.world);
	mCBuffer = DirectX11Core::createConstantBuffer(sizeof(CBufferWorld), 1, 0, &mCBData);
	
	if (header.vsInputLayout & VERTEX_ELEMENT::TEXCOORD) {
		std::vector<std::string> texs;
		int cnt = 0;
		file.read((char*)&cnt, sizeof(int));
		for (int k = 0; k < cnt; ++k) {
			char path[75];
			file.read(path, sizeof(char) * 75);
			std::string g = path;
			texs.push_back(g);
		}
		
		mSamplerState = DirectX11Core::createSamplerState();
		mTextureSRV = DirectX11Core::createShaderResourceViewFromFile(texs[0]);
	}
	file.close();

	return true;
}
void Renderable::draw()
{
	ID3D11Buffer *vbuffers[] = { mVBuffer };
	UINT strides[] = { mStride };
	UINT offsets[] = { mOffset };
	DirectX11Core::mDeviceContext->IASetVertexBuffers(0, 1, vbuffers, strides, offsets);

	DirectX11Core::mDeviceContext->IASetIndexBuffer(mIBuffer, DXGI_FORMAT_R32_UINT, 0);

	DirectX11Core::mDeviceContext->IASetInputLayout(mInputLayout);
	DirectX11Core::mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectX11Core::mDeviceContext->VSSetShader(mVShader, nullptr, 0);
	DirectX11Core::mDeviceContext->PSSetShader(mPShader, nullptr, 0);

	DirectX11Core::mDeviceContext->UpdateSubresource(mCBuffer, 0, 0, &mCBData, 0, 0);

	ID3D11Buffer *cbuffers[] = { mCBuffer };
	DirectX11Core::mDeviceContext->VSSetConstantBuffers(1, 1, cbuffers);

	if (mTextureSRV) {
		DirectX11Core::mDeviceContext->PSSetShaderResources(0, 1, &mTextureSRV);
	}
	if (mSamplerState) {
		DirectX11Core::mDeviceContext->PSSetSamplers(0, 1, &mSamplerState);
	}

	DirectX11Core::mDeviceContext->DrawIndexed(mIndiceCount, 0, 0);
}