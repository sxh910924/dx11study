#include <d3d11.h>
#include <D3DX11.h>
#include "GameSprite.h"

GameSprite::GameSprite()
	:rotation_(0)
{
	scale_.x = 1.f;
	scale_.y = 1.f;
}

GameSprite::~GameSprite()
{

}

XMMATRIX GameSprite::GetWorldMatrix()
{
	XMMATRIX translation = XMMatrixTranslation(position_.x, position_.y, 0.f);
	XMMATRIX rotationZ = XMMatrixRotationZ(rotation_);
	XMMATRIX scale = XMMatrixScaling(scale_.x, scale_.y, 1.f);
	return translation * rotationZ * scale;
}

void GameSprite::SetPosition(XMFLOAT2 &position)
{
	position_ = position;
}

void GameSprite::SetRotation(float rotation)
{
	rotation_ = rotation;
}

void GameSprite::SetScale(XMFLOAT2 &scale)
{
	scale_ = scale;
}
