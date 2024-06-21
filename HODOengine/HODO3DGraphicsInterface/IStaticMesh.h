#pragma once
#include <string>
#include "IRenderable.h"
#include "IMaterial.h"

namespace HDEngine
{
	class IStaticMesh : public IRenderable
	{
	public:
		virtual ~IStaticMesh() {}
		virtual void LoadMesh(const std::string& fileName) = 0;
		virtual void LoadMaterial(HDEngine::IMaterial* material, unsigned int element = 0) = 0;
		virtual void LoadAlbedoMap(const std::string& fileName, unsigned int element = 0) = 0;
		virtual void LoadNormalMap(const std::string& fileName, unsigned int element = 0) = 0;
		virtual void LoadARMMap(const std::string& fileName, unsigned int element = 0) = 0;
		virtual void LoadRoughnessMap(const std::string& fileName, unsigned int element = 0) = 0;
		virtual void LoadMetallicMap(const std::string& fileName, unsigned int element = 0) = 0;
		virtual void SetRoughnessValue(float value, unsigned int element = 0) = 0;
		virtual void SetMetallicValue(float value, unsigned int element = 0) = 0;
		virtual void SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element = 0) = 0;
		virtual void SetUseLight(bool useLight) = 0;
		virtual int GetMeshCount() = 0;
	};
}
