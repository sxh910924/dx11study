#pragma once

#include "Dx11DemoBase.h"
#include "GameSprite.h"

class GameSpriteDemo : public Dx11DemoBase
{
public:
	GameSpriteDemo();
	virtual ~GameSpriteDemo();

	virtual bool LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update(float dt) override;
	virtual void Render() override;

private:
	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;
	ID3D11ShaderResourceView* colorMap_;
	ID3D11SamplerState* colorMapSampler_;
	ID3D11BlendState* alphaBlendState_;
	ID3D11Buffer* mvpCB_;
	GameSprite sprites_[2];
	XMMATRIX vpMatrix_;
};