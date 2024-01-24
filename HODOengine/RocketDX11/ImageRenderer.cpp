﻿#include <locale>
#include <codecvt>
//#include <SimpleMath.h>

#include "ImageRenderer.h"
#include "ResourceManager.h"

#define FILEPATH "Resources/"

RocketCore::Graphics::ImageRenderer::ImageRenderer()
	: _xlocation(),
	_ylocation(),
	_scaleX(1.0f),
	_scaleY(1.0f),
	_imageWidth(),
	_imageHeight(),
	_active(true)
{
	_color = DirectX::Colors::White;
}

RocketCore::Graphics::ImageRenderer::~ImageRenderer()
{

}

void RocketCore::Graphics::ImageRenderer::SetImage(const std::string& filePath)
{
	std::string basePath = FILEPATH;

	std::string fullPath = basePath + filePath;

	std::wstring wideFilePath;

	// std::wstring_convert 대신 MultiByteToWideChar() 사용
	int requiredSize = MultiByteToWideChar(
		CP_UTF8,             // 문자열의 인코딩 방식 (UTF-8)
		0,                   // 플래그 (여기서는 0 사용)
		fullPath.c_str(),    // 변환할 문자열
		-1,                  // 변환할 문자열의 길이 (-1이면 널 종료 문자까지 변환)
		nullptr,             // 출력 버퍼 (크기 계산을 위해 nullptr 전달)
		0                    // 출력 버퍼의 크기 (0이면 크기 계산만 수행)
	);

	if (requiredSize > 0)
	{
		wideFilePath.resize(requiredSize);
		MultiByteToWideChar(
			CP_UTF8,
			0,
			fullPath.c_str(),
			-1,
			&wideFilePath[0],
			requiredSize
		);
	}

	DirectX::CreateWICTextureFromFile(
		_device,
		_deviceContext,
		wideFilePath.c_str(),
		nullptr,
		_imagerSRV.GetAddressOf());

	D3D11_TEXTURE2D_DESC textureDesc;
	ID3D11Resource* resource;
	_imagerSRV->GetResource(&resource);
	ID3D11Texture2D* texture2D = static_cast<ID3D11Texture2D*>(resource);
	texture2D->GetDesc(&textureDesc);

	_imageWidth = textureDesc.Width;
	_imageHeight = textureDesc.Height;

	_centerX = _imageWidth / 2.0f;
	_centerY = _imageHeight / 2.0f;
}

void RocketCore::Graphics::ImageRenderer::SetScreenSpacePosition(float x, float y)
{
	_xlocation;
	_ylocation;
	//_xlocation = x - _centerX;
	//_ylocation = y - _centerY;
}

void RocketCore::Graphics::ImageRenderer::SetWorldSpace()
{

}

void RocketCore::Graphics::ImageRenderer::Render(DirectX::SpriteBatch* spriteBatch)
{
	if (_active == true)
	{
		spriteBatch->Begin();

		spriteBatch->Draw(
			_imagerSRV.Get(),
			DirectX::XMFLOAT2(_xlocation - _centerX, _ylocation - _centerY),
			nullptr,
			_color,
			0.0f,										//회전 각도
			DirectX::XMFLOAT2(0.5f, 0.5f),				//  이미지의 원점->0.0f,0.0f이면 좌측상단
			DirectX::XMFLOAT2(_scaleX, _scaleY));		// 이미지 스케일

		spriteBatch->End();
	}

}

void RocketCore::Graphics::ImageRenderer::SetWorldTM(const Matrix& worldTM)
{
	_xlocation = worldTM._41;
	_ylocation = worldTM._42;
}

void RocketCore::Graphics::ImageRenderer::SetScreenSpace()
{

}

void RocketCore::Graphics::ImageRenderer::SetActive(bool isActive)
{
	_active = isActive;
}

void RocketCore::Graphics::ImageRenderer::InitalizeImageRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	_device = device;
	_deviceContext = deviceContext;
}


void RocketCore::Graphics::ImageRenderer::ChangeScale(float x, float y)
{
	_scaleX = x;
	_scaleY = y;
	_imageWidth *= _scaleX;
	_imageHeight *= _scaleY;
}

float RocketCore::Graphics::ImageRenderer::GetScreenSpacePositionX()
{
	return _xlocation;
}

float RocketCore::Graphics::ImageRenderer::GetScreenSpacePositionY()
{
	return _ylocation;
}

float RocketCore::Graphics::ImageRenderer::GetWidth()
{
	return _imageWidth;
}

float RocketCore::Graphics::ImageRenderer::GetHeight()
{
	return _imageHeight;
}

DirectX::FXMVECTOR RocketCore::Graphics::ImageRenderer::SetColor(DirectX::FXMVECTOR color)
{
	return _color = color;
}
