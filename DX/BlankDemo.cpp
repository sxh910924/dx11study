
#include "BlankDemo.h"

BlankDemo::BlankDemo()
{

}

BlankDemo::~BlankDemo()
{

}

bool BlankDemo::LoadContent()
{
	return true;
}

void BlankDemo::UnloadContent()
{

}

void BlankDemo::Update(float dt)
{

}

void BlankDemo::Render()
{
	if (d3dContext_ == nullptr)
		return;

	float clearColor[4] = { 0.f,0.f,0.25f,1.f };
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);

	swapChain_->Present(0, 0);
}

