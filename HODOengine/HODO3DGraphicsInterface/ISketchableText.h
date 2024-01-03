#pragma once
#include "ISketchable.h"
#include <windows.h>
#include <string>

namespace HDEngine
{
	class ISketchableText : public ISketchable
	{
	public:
		virtual void SetText(TCHAR* text) = 0;
		virtual const std::string GetText() = 0;
		virtual void SetActive(bool active) = 0;
		virtual void SetScreenSpace() = 0;
		virtual void SetWorldSpace() = 0;
	};
}