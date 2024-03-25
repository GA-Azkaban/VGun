#pragma once
#include <string>
#include <windows.h>
#include "IRenderable.h"

namespace HDEngine
{
	class ISkinnedMesh : public IRenderable
	{
	public:
		virtual void LoadMesh(const std::string& fileName) = 0;
		virtual void LoadNormalMap(const std::string& fileName) = 0;
		virtual void LoadDiffuseMap(const std::string& fileName) = 0;
		virtual void LoadARMMap(const std::string& fileName) = 0;
		virtual void PlayAnimation(const std::string& fileName, bool isLoop = true) = 0;
		virtual bool IsAnimationEnd() = 0;
		virtual void SetOutlineActive(bool isActive) = 0;
	};
}
