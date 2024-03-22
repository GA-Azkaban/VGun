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
		virtual void LoadRoughnessMap(const std::string& fileName) = 0;
		virtual void LoadMetallicMap(const std::string& fileName) = 0;
		virtual void SetRoughnessValue(float value) = 0;
		virtual void SetMetallicValue(float value) = 0;
	};
}
