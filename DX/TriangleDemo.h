#pragma once

#include "Dx11DemoBase.h"

class TriangleDemo : public Dx11DemoBase
{
public:

	TriangleDemo();
	virtual ~TriangleDemo();

	virtual bool LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update(float dt) override;
	virtual void Render() override;

private:

	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;

};
