#pragma once

#include <memory>
#include <DirectXColors.h>
#include <string>

#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"

namespace RocketCore::Graphics
{
	class D2DTexTRenderer : public HDEngine::ISketchableText
	{
	public:
		D2DTexTRenderer();
		~D2DTexTRenderer();

		virtual void SetText(const std::string& str) override;

		virtual void SetFont(const std::string& str) override;

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

		virtual void FadeIn() override;
		virtual void FadeOut() override;
		virtual bool GetComplete() override;
		virtual bool GetFadeMode() override;
		virtual bool SetFadeMode(bool isFade) override;

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
		float _xLocation;
		float _yLocation;
		float _zLocation;

		DirectX::XMVECTOR _size;
		DirectX::XMVECTOR _measureLong;
		float _width;
		float _height;

		bool _receiveTMInfoFlag;
		float _sortOrder;
	};
}
