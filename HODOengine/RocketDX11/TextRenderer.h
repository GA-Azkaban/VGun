#pragma once

#include <d3d11.h>
#include <memory>
#include <DirectXColors.h>
#include <string>
#include <DXTK/SpriteFont.h>

#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"

namespace RocketCore::Graphics
{
	/// <summary>
	/// 텍스트랜더러 클래스
	/// </summary>
	class TextRenderer : public HDEngine::ISketchableText
	{
	public:
		TextRenderer();
		~TextRenderer();

		virtual void SetText(const std::string& str) override;

		virtual void SetFloatValue(const float value) override;

		virtual void SetintValue(const int value) override;

		virtual void SetColor(DirectX::FXMVECTOR color) override;

		virtual const std::string GetText() override;

		virtual void SetScreenSpacePosition(float x, float y) override;

		virtual void SetScreenSpace() override;

		virtual void SetWorldSpace() override;

		virtual void SetWorldTM(const Matrix& worldTM) override;

		virtual void SetActive(bool isActive) override;

		virtual bool GetActive() override;

		virtual float GetWidth() override;
		virtual float GetHeight() override;

	private:
		void MeasureTextSize();

	public:
		void Render(DirectX::SpriteBatch* spriteBatch);
		void RenderDebug(DirectX::SpriteBatch* spriteBatch);

		DirectX::SpriteFont* _font;

	private:
		// 내부 변수들
		std::string _str;
		float _renderFloat;
		int _renderInt;

		DirectX::XMVECTOR _color;
		float _xLocation;
		float _yLocation;
		float _zLocation;

		DirectX::XMVECTOR _size;
		DirectX::XMVECTOR _measureLong;
		float _width;
		float _height;

		bool _isActive;
	};
}
