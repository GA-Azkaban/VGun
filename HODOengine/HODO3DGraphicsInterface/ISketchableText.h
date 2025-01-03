﻿#pragma once
#include <windows.h>
#include <string>
#include "MathHeader.h"
#include "ISketchable.h"

namespace HDEngine
{
	class ISketchableText : public ISketchable
	{
	public:
		virtual void SetText(const std::string& str) = 0;
		virtual void SetFont(const std::string& str) = 0;
		virtual void SetFloatValue(const float value) = 0;
		virtual void SetintValue(const int value) = 0;
		virtual void SetScreenSpacePosition(float x, float y) = 0;
		virtual void SetColor(DirectX::FXMVECTOR color) = 0;
		virtual void SetAlpha(float alpha) = 0;
		virtual void SetActive(bool active) = 0;
		virtual bool GetActive() = 0;

		virtual void ChangeScale(float size) = 0;

		virtual void SetDefaultColor(DirectX::FXMVECTOR color) = 0;
		virtual void ReturnDefaultColor() = 0;

		virtual void SetWorldSpace() = 0;

		virtual const std::string GetText() = 0;
		

	};
}
