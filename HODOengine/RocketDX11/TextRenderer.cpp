#include "TextRenderer.h"
#include "ResourceManager.h"
#include "MathHeader.h"

namespace RocketCore::Graphics
{
	TextRenderer::TextRenderer()
		: _renderFloat(),
		_renderInt(),
		_xLocation(),
		_yLocation()
	{
		_font = ResourceManager::Instance().GetDefaultFont();
		_str = "Default Text";
		_color = DirectX::Colors::White;
		_isTranslated = false;

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
		if (_isTranslated != true)
		{
			_xLocation = worldTM._41;
			_yLocation = worldTM._42;
		}
	}

	void TextRenderer::SetActive(bool isActive)
	{
		isActive = 1;
	}

	void TextRenderer::SetColor(Color color)
	{
		_color = color;
	}

	void TextRenderer::SetScreenSpacePosition(float x, float y)
	{
		_xLocation = x;
		_yLocation = y;

		_isTranslated = true;
	}

	void TextRenderer::Render(DirectX::SpriteBatch* spriteBatch)
	{
		std::wstring wstr(_str.begin(), _str.end());
		_font->DrawString(spriteBatch, wstr.c_str(), DirectX::XMFLOAT2(_xLocation, _yLocation), _color);
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
		return {};
	}

	float TextRenderer::GetHeight()
	{
		return {};
	}
}
