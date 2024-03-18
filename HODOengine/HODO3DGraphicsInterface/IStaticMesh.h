#pragma once
#include <string>
#include "IRenderable.h"

namespace HDEngine
{
	class IStaticMesh : public IRenderable
	{
	public:
		virtual ~IStaticMesh() {}
		virtual void LoadMesh(const std::string& fileName) = 0;
		virtual void LoadNormalMap(const std::string& fileName) = 0;
		virtual void LoadDiffuseMap(const std::string& fileName) = 0;
		virtual void LoadARMMap(const std::string& fileName) = 0;
		virtual void SetOutlineActive(bool isActive) = 0;
		virtual void SetOutlineData(const Vector4& color = Vector4{ 1.0f, 0.54f, 0.0f, 1.0f }, bool depthCheck = true) = 0;
	};
}
