﻿#pragma once

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

		virtual void SetFont(const std::string& str) override;

		virtual void SetFloatValue(const float value) override;

		virtual void SetintValue(const int value) override;

		virtual void SetColor(DirectX::FXMVECTOR color) override;

		virtual void SetAlpha(float alpha) override;

		virtual void SetDefaultColor(DirectX::FXMVECTOR color)override;

		virtual void ReturnDefaultColor()override;

		virtual const std::string GetText() override;

		virtual void SetScreenSpacePosition(float x, float y) override;

		virtual void SetScreenSpace() override;

		virtual void SetWorldSpace() override;

		virtual void SetWorldTM(const Matrix& worldTM) override;

		virtual void SetActive(bool isActive) override;

		virtual bool GetActive() override;

		virtual void ChangeScale(float size) override;

		virtual float GetWidth() override;
		virtual float GetHeight() override;

		virtual void FadeIn(float time) override;
		virtual void FadeOut(float time) override;
		virtual bool GetComplete() override;
		virtual bool GetFadeMode() override;

		virtual void SetSortOrder(float order) override;
		float GetSortOrder() const { return _sortOrder; }

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
		bool _isActive;

		DirectX::XMVECTOR _color;
		DirectX::XMVECTOR _defaultColor;
		float _xLocation;
		float _yLocation;
		float _zLocation;

		float _size;
		float _width;
		float _height;

		bool _receiveTMInfoFlag;
		float _sortOrder;
	};
}
