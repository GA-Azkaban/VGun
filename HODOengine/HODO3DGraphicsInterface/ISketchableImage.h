#pragma once
#include <string>
#include "ISketchable.h"

namespace HDEngine
{
	class ISketchableImage : public ISketchable
	{
	public:
		virtual void SetImage(const std::string& filePath) = 0;
		virtual void SetScreenSpacePosition(float x, float y) = 0;
		virtual float GetScreenSpacePositionX() = 0;
		virtual float GetScreenSpacePositionY() = 0;
		virtual DirectX::FXMVECTOR SetColor(DirectX::FXMVECTOR color) = 0;
		virtual float GetWidth() override = 0;
		virtual float GetHeight() override = 0;
		virtual void ChangeScale(float x, float y) = 0;
		virtual void SetAngle(float angle) = 0;
		virtual void SetOrigin(float x, float y) = 0;
		virtual void SetCenter(float x, float y) = 0;

		virtual void SetDefalutColor(DirectX::FXMVECTOR color) = 0;
		virtual	void RetunDefalutColor() = 0;
		
		// Fade
		virtual void FadeIn(float time) override = 0;
		virtual void FadeOut(float time) override = 0;
		virtual bool GetFadeMode() override = 0;
		virtual bool GetComplete() override = 0;

	};
}
