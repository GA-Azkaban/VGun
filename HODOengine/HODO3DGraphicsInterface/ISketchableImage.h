#pragma once
#include "ISketchable.h"
#include "..\\HODOmath\HODOmath.h"
#include <string>

namespace HDEngine
{
	class ISketchableImage : public ISketchable
	{
	public:
		virtual void SetImage(const std::string& filePath) = 0;
		virtual void SetScreenSpacePosition(float x, float y) = 0;
		virtual float GetScreenSpacePositionX() = 0;
		virtual float GetScreenSpacePositionY() = 0;
		virtual void SetWorldSpace() = 0;
		virtual float GetWidth() = 0;
		virtual float GetHeight() = 0;
		virtual void ChangeScale(float x, float y) = 0;
	};
}