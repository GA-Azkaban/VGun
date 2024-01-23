#pragma once
#include <string>
#include "IRenderable.h"
#include <windows.h>

namespace HDEngine
{
	class ISkinnedMesh : public IRenderable
	{
	public:
		virtual void LoadMesh(const std::string& fileName) = 0;
		virtual void LoadNormalMap(const std::string& fileName) = 0;
		virtual void LoadDiffuseMap(const std::string& fileName) = 0;
		virtual void PlayAnimation(const std::string& animName, bool isLoop = true) = 0;
		virtual void PlayAnimation(UINT index, bool isLoop = true) = 0;
	};
}