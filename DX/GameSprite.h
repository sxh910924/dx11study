#pragma once
#include "Dx11DemoBase.h"
#include <xnamath.h>

//using namespace DirectX;

class GameSprite
{
public:
	GameSprite();
	virtual ~GameSprite();

	XMMATRIX GetWorldMatrix();

	void SetPosition(XMFLOAT2 &posiotion);
	void SetRotation(float rotation);
	void SetScale(XMFLOAT2 &scale);

private:
	XMFLOAT2 position_;
	float rotation_;
	XMFLOAT2 scale_;
};

