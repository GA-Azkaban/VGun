#include "TextRenderer.h"
#include "ResourceManager.h"
#include "..\\HODOmath\\HODOmath.h"

RocketCore::Graphics::TextRenderer::TextRenderer()
	: _renderFloat(),
	_renderInt(),
	_xLocation(),
	_yLocation()
{
	_font = ResourceManager::Instance().GetDefaultFont();
	_str = "Default Text";
	_color = DirectX::Colors::White;
	_isPositionedDiffrent = false;

}

RocketCore::Graphics::TextRenderer::~TextRenderer()
{

}

void RocketCore::Graphics::TextRenderer::SetText(const std::string& str)
{
	_str = str;
}

const std::string RocketCore::Graphics::TextRenderer::GetText()
{
	return _str;
}

void RocketCore::Graphics::TextRenderer::SetScereenSpace()
{

}

void RocketCore::Graphics::TextRenderer::SetWorldSpace()
{

}

void RocketCore::Graphics::TextRenderer::SetWorldTM(const HDMath::HDFLOAT4X4& worldTM)
{
	if (_isPositionedDiffrent != true)
	{
		_xLocation = worldTM._41;
		_yLocation = worldTM._42;
	}
}

void RocketCore::Graphics::TextRenderer::SetActive(bool isActive)
{
	isActive = 1;
}

void RocketCore::Graphics::TextRenderer::SetColor(DirectX::FXMVECTOR color)
{
	_color = color;
}

void RocketCore::Graphics::TextRenderer::SetScreenSpacePosition(float x, float y)
{
	_xLocation = x;
	_yLocation = y;

	_isPositionedDiffrent = true;
}

void RocketCore::Graphics::TextRenderer::Render(DirectX::SpriteBatch* spriteBatch)
{
	std::wstring wstr(_str.begin(), _str.end());
	_font->DrawString(spriteBatch, wstr.c_str(), DirectX::XMFLOAT2(_xLocation, _yLocation), _color);
}


void RocketCore::Graphics::TextRenderer::SetFloatValue(const float value)
{
	_renderFloat = value;
}

void RocketCore::Graphics::TextRenderer::SetintValue(const int value)
{
	_renderInt = value;
}
