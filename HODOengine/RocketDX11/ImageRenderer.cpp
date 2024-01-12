#include <locale>
#include <codecvt>
//#include <SimpleMath.h>

#include "ImageRenderer.h"
#include "ResourceManager.h"


RocketCore::Graphics::ImageRenderer::ImageRenderer()
	: _xlocation(),
	_ylocation(),
	_scaleX(1.0f),
	_scaleY(1.0f)
{
	_color = DirectX::Colors::White;
}

RocketCore::Graphics::ImageRenderer::~ImageRenderer()
{

}

void RocketCore::Graphics::ImageRenderer::SetImage(const std::string& filePath)
{
	_str = filePath;
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;	// <locale>
	//std::wstring wideFilePath = converter.from_bytes(filePath);	// <codecvt>
	std::wstring wideFilePath;

	// std::wstring_convert 대신 MultiByteToWideChar() 사용
	int requiredSize = MultiByteToWideChar(
		CP_UTF8,             // 문자열의 인코딩 방식 (UTF-8)
		0,                   // 플래그 (여기서는 0 사용)
		filePath.c_str(),    // 변환할 문자열
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
			filePath.c_str(),
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
}

void RocketCore::Graphics::ImageRenderer::SetScreenSpacePosition(float x, float y)
{
	_xlocation = x;
	_ylocation = y;
	_isTranslated = true;
}

void RocketCore::Graphics::ImageRenderer::SetWorldSpace()
{

}

void RocketCore::Graphics::ImageRenderer::Render(DirectX::SpriteBatch* spriteBatch)
{
	spriteBatch->Begin();
<<<<<<< HEAD
	//spriteBatch->Draw(_imagerSRV.Get(), DirectX::XMFLOAT2(_xlocation,_ylocation), nullptr, _color);
	spriteBatch->Draw(
		_imagerSRV.Get(),
		DirectX::XMFLOAT2(_xlocation, _ylocation),
		nullptr,
		_color,
		0.0f,										//회전 각도
		DirectX::XMFLOAT2(0.0f, 0.0f),				//  이미지의 원점->0.0f,0.0f이면 좌측상단
		DirectX::XMFLOAT2(_scaleX,_scaleY));		// 이미지 스케일
=======
	spriteBatch->Draw(_imagerSRV.Get(), DirectX::XMFLOAT2(_xlocation, _ylocation), nullptr, _color);
>>>>>>> ab573bef3ee680cc888d7abe33ca669fae428aea
	spriteBatch->End();
}

void RocketCore::Graphics::ImageRenderer::SetWorldTM(const HDMath::HDFLOAT4X4& worldTM)
{
	if (_isTranslated != true)
	{
		_xlocation = worldTM._41;
		_ylocation = worldTM._42;
	}
}

void RocketCore::Graphics::ImageRenderer::SetScereenSpace()
{

}

void RocketCore::Graphics::ImageRenderer::SetActive(bool isActive)
{
	isActive = 1;
}

void RocketCore::Graphics::ImageRenderer::InitalizeImageRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	_device = device;
	_deviceContext = deviceContext;
}

<<<<<<< HEAD
void RocketCore::Graphics::ImageRenderer::ChangeScale(float x, float y)
{
	_scaleX = x;
	_scaleY = y;
}
=======
float RocketCore::Graphics::ImageRenderer::GetScreenSpacePositionX()
{
	return _xlocation;
}

float RocketCore::Graphics::ImageRenderer::GetScreenSpacePositionY()
{
	return _ylocation;
}

//float RocketCore::Graphics::ImageRenderer::GetScreenSpacePosition()
//{
//	return _xlocation;
//}

>>>>>>> ab573bef3ee680cc888d7abe33ca669fae428aea
