
#include "GameSpriteDemo.h"


struct VertexPos
{
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
};

GameSpriteDemo::GameSpriteDemo()
	: solidColorVS_(nullptr)
	, solidColorPS_(nullptr)
	, inputLayout_(nullptr)
	, vertexBuffer_(nullptr)
	, colorMap_(nullptr)
	, colorMapSampler_(nullptr)
	, alphaBlendState_(nullptr)
	, mvpCB_(nullptr)
	, vpMatrix_(XMMATRIX())
{

}

GameSpriteDemo::~GameSpriteDemo()
{

}

bool GameSpriteDemo::LoadContent()
{
	// 复制TextureDemo.cpp中LoadContent的内容，开始
	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileD3DShader("TextureMap_1.fx", "VS_Main", "vs_4_0", &vsBuffer);

	if (compileResult == false)
	{
		//DXTRACE_MSG("Error compiling the vertex shader!");
		return false;
	}

	HRESULT d3dResult;

	d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), 0, &solidColorVS_);

	if (FAILED(d3dResult))
	{
		//DXTRACE_MSG("Error creating the vertex shader!");
		if (vsBuffer) vsBuffer->Release();
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	d3dResult = d3dDevice_->CreateInputLayout(solidColorLayout, totalLayoutElements,
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout_);

	vsBuffer->Release();

	if (FAILED(d3dResult))
	{
		//DXTRACE_MSG("Error creating the input layout!");
		return false;
	}

	ID3DBlob* psBuffer = 0;

	compileResult = CompileD3DShader("TextureMap_1.fx", "PS_Main", "ps_4_0", &psBuffer);

	if (compileResult == false)
	{
		//DXTRACE_MSG("Error compiling pixel shader!");
		return false;
	}

	d3dResult = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(), 0, &solidColorPS_);

	psBuffer->Release();

	if (FAILED(d3dResult))
	{
		//DXTRACE_MSG("Error creating pixel shader!");
		return false;
	}


	d3dResult = D3DX11CreateShaderResourceViewFromFile(d3dDevice_,
		"decal_1.dds", 0, 0, &colorMap_, 0);

	if (FAILED(d3dResult))
	{
		//DXTRACE_MSG("Failed to load the texture image!");
		return false;
	}

	D3D11_SAMPLER_DESC colorMapDesc;
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	d3dResult = d3dDevice_->CreateSamplerState(&colorMapDesc, &colorMapSampler_);

	if (FAILED(d3dResult))
	{
		//DXTRACE_MSG("Failed to create color map sampler state!");
		return false;
	}

	// 复制TextureDemo.cpp中LoadContent的内容，结束
	ID3D11Resource* colorTex;
	colorMap_->GetResource(&colorTex);

	D3D11_TEXTURE2D_DESC colorTexDesc;
	((ID3D11Texture2D*)colorTex)->GetDesc(&colorTexDesc);
	colorTex->Release();

	float halfWidth = (float)colorTexDesc.Width / 2.f;
	float halfHeight = (float)colorTexDesc.Height / 2.f;

	VertexPos vertices[] =
	{
		{ XMFLOAT3( halfWidth,  halfHeight, 1.f), XMFLOAT2( 1.f, 0.f) },
		{ XMFLOAT3( halfWidth, -halfHeight, 1.f), XMFLOAT2( 1.f, 0.f) },
		{ XMFLOAT3(-halfWidth, -halfHeight, 1.f), XMFLOAT2( 0.f, 1.f) },
		{ XMFLOAT3(-halfWidth, -halfHeight, 1.f), XMFLOAT2( 0.f, 1.f) },
		{ XMFLOAT3(-halfWidth,  halfHeight, 1.f), XMFLOAT2( 0.f, 0.f) },
		{ XMFLOAT3( halfWidth,  halfHeight, 1.f), XMFLOAT2( 1.f, 0.f) }//顶点没重吗，这个，2333，好难
	};

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 6;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc,&resourceData,&vertexBuffer_);
	if (d3dResult)
	{
		//DXTRACE_MSG("Failed to create vertex buffer!");
		return false;
	}

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory(&constDesc, sizeof(constDesc));
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof(XMMATRIX);
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &mvpCB_);

	if (FAILED(d3dResult))
	{
		return false;
	}

	XMFLOAT2 sprite1Pos(100.f,300.f);
	sprites_[0].SetPosition(sprite1Pos);

	XMFLOAT2 sprite2Pos(400.f,100.f);
	sprites_[1].SetPosition(sprite2Pos);

	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.f, 800.f, 0.f, 600.f, 0.1f, 100.f);
	vpMatrix_ = XMMatrixMultiply(view, projection);

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	float blendFactor[4] = { 0.f,0.f,0.f,0.f };

	d3dDevice_->CreateBlendState(&blendDesc, &alphaBlendState_);
	d3dContext_->OMSetBlendState(alphaBlendState_, blendFactor, 0xFFFFFFFF);

	return true;
}

void GameSpriteDemo::UnloadContent()
{
	if (solidColorVS_)solidColorVS_->Release();
	if (solidColorPS_)solidColorPS_->Release();
	if (inputLayout_)inputLayout_->Release();
	if (vertexBuffer_)vertexBuffer_->Release();
	if (colorMap_)colorMap_->Release();
	if (colorMapSampler_)colorMapSampler_->Release();
	if (alphaBlendState_)alphaBlendState_->Release();
	if (mvpCB_)mvpCB_->Release();

	solidColorVS_ = nullptr;
	solidColorPS_ = nullptr;
	inputLayout_ = nullptr;
	vertexBuffer_ = nullptr;
	colorMap_ = nullptr;
	colorMapSampler_ = nullptr;
	alphaBlendState_ = nullptr;
	mvpCB_ = nullptr;
}

void GameSpriteDemo::Update(float dt)
{

}

void GameSpriteDemo::Render()
{
	if (d3dContext_ == nullptr) return;

	float clearColor[4] = { 0.f,0.f,0.25f,1.f };
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);

	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

	d3dContext_->IASetInputLayout(inputLayout_);
	d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext_->VSSetShader(solidColorVS_, 0, 0);
	d3dContext_->PSSetShader(solidColorPS_, 0, 0);
	d3dContext_->PSSetShaderResources(0, 1, &colorMap_);
	d3dContext_->PSSetSamplers(0, 1, &colorMapSampler_);

	for (int i = 0; i < 2; i++)
	{
		XMMATRIX world = sprites_[i].GetWorldMatrix();
		XMMATRIX mvp = XMMatrixMultiply(world, vpMatrix_);
		mvp = XMMatrixTranspose(mvp);
		d3dContext_->UpdateSubresource(mvpCB_, 0, 0, &mvp, 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &mvpCB_);
		d3dContext_->Draw(6, 0);
	}
	swapChain_->Present(0, 0);
}

