﻿#pragma once
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
		virtual void PlayAnimation(const std::string& fileName, bool isLoop = true) = 0;
		virtual bool IsAnimationEnd() = 0;
		virtual void SetOutlineActive(bool isActive) = 0;
		virtual void SetOutlineData(const Vector4& color = Vector4{ 1.0f, 0.54f, 0.0f, 1.0f }, bool depthCheck = true) = 0;
	};
}
