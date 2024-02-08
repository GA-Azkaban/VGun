﻿#include "TextRenderer.h"
#include "ResourceManager.h"
#include "MathHeader.h"

namespace RocketCore::Graphics
{
	TextRenderer::TextRenderer()
		: _renderFloat(),
		_renderInt(),
		_xLocation(),
		_yLocation(),
		_zLocation(),
		_size(),
		_width(),
		_height(),
		_isActive(true)
	{
		_font = ResourceManager::Instance().GetDefaultFont();
		_str = "Default Text";
		_color = DirectX::Colors::White;
	}

	TextRenderer::~TextRenderer()
	{

	}

	void TextRenderer::SetText(const std::string& str)
	{
		_str = str;
	}

	const std::string TextRenderer::GetText()
	{
		return _str;
	}

	void TextRenderer::SetScreenSpace()
	{

	}

	void TextRenderer::SetWorldSpace()
	{

	}

	void TextRenderer::SetWorldTM(const Matrix& worldTM)
	{
		_xLocation = worldTM._41;
		_yLocation = worldTM._42;
		_zLocation = worldTM._43;	// for Debugging text
	}

	void TextRenderer::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void TextRenderer::SetColor(DirectX::FXMVECTOR color)
	{
		_color = color;
	}

	void TextRenderer::SetScreenSpacePosition(float x, float y)
	{
		_xLocation = x;
		_yLocation = y;
	}

	void TextRenderer::Render(DirectX::SpriteBatch* spriteBatch)
	{
		if (_isActive)
		{
			MeasureTextSize();	//여기두면 안되는데

			std::wstring wstr(_str.begin(), _str.end());
			_font->DrawString(
				spriteBatch,
				wstr.c_str(),
				DirectX::XMFLOAT2(_xLocation, _yLocation),
				_color);
		}
	}

	void TextRenderer::SetFloatValue(const float value)
	{
		_renderFloat = value;
	}

	void TextRenderer::SetintValue(const int value)
	{
		_renderInt = value;
	}

	float TextRenderer::GetWidth()
	{
		return _width;
	}

	float TextRenderer::GetHeight()
	{
		return _height;
	}

	void TextRenderer::MeasureTextSize()
	{
		std::wstring wstr(_str.begin(), _str.end());
		_size = _font->MeasureString(wstr.c_str());
		_width = DirectX::XMVectorGetX(_size);
		_height = DirectX::XMVectorGetY(_size);

		_yLocation -= _height * 0.5f;
		_xLocation -= _width * 0.5f;
	}

	void TextRenderer::RenderDebug(DirectX::SpriteBatch* spriteBatch)
	{

	}

	bool TextRenderer::GetActive()
	{
		return _isActive;
	}

}
