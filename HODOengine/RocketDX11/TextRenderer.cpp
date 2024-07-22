#include "TextRenderer.h"
#include "ResourceManager.h"
#include "MathHeader.h"

#define FONTPATH "Resources/Font/"

namespace RocketCore::Graphics
{
	TextRenderer::TextRenderer()
		: _renderFloat(),
		_renderInt(),
		_xLocation(),
		_yLocation(),
		_zLocation(),
		_size(1.0f),
		_width(),
		_height(),
		_isActive(true),
		_receiveTMInfoFlag(false),
		_sortOrder(0)
	{
		_font = ResourceManager::Instance().GetDefaultFont();
		_str = "";
		_color = DirectX::Colors::White;
		_defaultColor = DirectX::Colors::White;
	}

	TextRenderer::~TextRenderer()
	{

	}

	void TextRenderer::SetText(const std::string& str)
	{
		_str = str;
		MeasureTextSize();
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

		_yLocation -= _height * 0.5f;
		_xLocation -= _width * 0.5f;

		_receiveTMInfoFlag = true;
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
		if (!_isActive)
			return;

		if (_receiveTMInfoFlag)
		{
			std::wstring wstr(_str.begin(), _str.end());
			_font->DrawString(
				spriteBatch,
				wstr.c_str(),
				DirectX::XMFLOAT2(_xLocation, _yLocation),
				_color,
				0.0f,
				{0.0f,0.0f},
				_size,
				DirectX::DX11::SpriteEffects_None,
				_sortOrder
			);

		}
		_receiveTMInfoFlag = false;
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
		DirectX::XMVECTOR measure = _font->MeasureString(wstr.c_str());
		_width = DirectX::XMVectorGetX(measure);
		_height = DirectX::XMVectorGetY(measure);
	}

	void TextRenderer::RenderDebug(DirectX::SpriteBatch* spriteBatch)
	{

	}

	bool TextRenderer::GetActive()
	{
		return _isActive;
	}

	void TextRenderer::ChangeScale(float size)
	{
		_size = size;
	}

	void TextRenderer::SetSortOrder(float order)
	{
		_sortOrder = order;
	}

	void TextRenderer::FadeIn(float time)
	{

	}

	void TextRenderer::FadeOut(float time)
	{

	}

	bool TextRenderer::GetFadeMode()
	{
		return true;
	}

	bool TextRenderer::GetComplete()
	{
		return true;
	}

	void TextRenderer::SetFont(const std::string& str)
	{
		_font = ResourceManager::Instance().SetFont(str);
	}

	void TextRenderer::SetDefaultColor(DirectX::FXMVECTOR color)
	{
		_defaultColor = color;
	}

	void TextRenderer::ReturnDefaultColor()
	{
		_color = _defaultColor;
	}

}
