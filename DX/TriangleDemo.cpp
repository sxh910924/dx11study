#include "TriangleDemo.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

struct VertexPos
{
	XMFLOAT3 pos;
};

TriangleDemo::TriangleDemo()
	: solidColorVS_(nullptr)
	, solidColorPS_(nullptr)
	, inputLayout_(nullptr)
	, vertexBuffer_(nullptr)
{

}

TriangleDemo::~TriangleDemo()
{

}

bool TriangleDemo::LoadContent()
{
	ID3DBlob* vsBuffer = nullptr;
	char* fxName = new char[19];
	strcpy_s(fxName, 19, "SolidGreenColor.fx");
	char* entry = new char[8];
	strcpy_s(entry, 8, "VS_Main");
	char* shaderModel = new char[7];
	strcpy_s(shaderModel, 7, "vs_4_0");

	bool compileResult = CompileD3DShader(fxName, entry, shaderModel, &vsBuffer);

	if (compileResult == false)
	{
		MessageBox(0,"Error loading vertex shader!","Compile Error",MB_OK);
		return false;
	}

	HRESULT d3dResult;
	d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		0,
		&solidColorVS_);

	if (FAILED(d3dResult))
	{
		if (vsBuffer)
		{
			vsBuffer->Release();
		}
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] = 
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);
	d3dResult = d3dDevice_->CreateInputLayout(solidColorLayout,
		totalLayoutElements,
		vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		&inputLayout_);

	vsBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}

	ID3DBlob* psBuffer = nullptr;

	strcpy_s(entry, 8, "PS_Main");
	strcpy_s(shaderModel, 7, "ps_4_0");
	compileResult = CompileD3DShader(fxName, entry, shaderModel, &psBuffer);
	if (compileResult==false)
	{
		MessageBox(0,"Error loading pixel shader!","Compile Error",MB_OK);
		return false;
	}

	d3dResult = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(), 0, &solidColorPS_);
	psBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}

	delete[] fxName;
	fxName = nullptr;
	delete[] entry;
	entry = nullptr;
	delete[] shaderModel;
	shaderModel = nullptr;

	//part 2
	VertexPos vertices[]=
	{
		XMFLOAT3(0.5f,0.5f,0.5f),
		XMFLOAT3(0.5f,-0.5f,0.5f),
		XMFLOAT3(-0.5f,-0.5f,0.5f)
	};

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));

	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 3;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData,sizeof(resourceData));
	resourceData.pSysMem = vertices;

	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc,
		&resourceData,
		&vertexBuffer_);

	if (FAILED(d3dResult))
	{
		return false;
	}

	return true;
}

void TriangleDemo::UnloadContent()
{
	if (solidColorVS_) solidColorVS_->Release();
	if (solidColorPS_)solidColorPS_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	solidColorVS_ = nullptr;
	solidColorPS_ = nullptr;
	inputLayout_ = nullptr;
	vertexBuffer_ = nullptr;
}

void TriangleDemo::Update(float dt)
{

}

void TriangleDemo::Render()
{
	if (d3dContext_==nullptr)
	{
		return;
	}

	float clearColor[4] = {0.f,0.f,0.25f,1.f};
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);

	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

	d3dContext_->IASetInputLayout(inputLayout_);
	d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext_->VSSetShader(solidColorVS_, 0, 0);
	d3dContext_->PSSetShader(solidColorPS_, 0, 0);
	d3dContext_->Draw(3, 0);
	swapChain_->Present(0, 0);
}
