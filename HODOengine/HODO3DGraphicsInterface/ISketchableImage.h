#pragma once
#include "ISketchable.h"
#include <string>

namespace HDEngine
{
	class ISketchableImage : public ISketchable
	{
	public:
		virtual void SetImage(const std::string& filePath) = 0;
		virtual void SetScreenSpacePosition(float x, float y) = 0;
		virtual void SetWorldSpace() = 0;
	};
}