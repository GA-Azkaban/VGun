#pragma once
#include "RendererBase.h"
#include "dllExporter.h"
#include "../HODO3DGraphicsInterface/IStaticMesh.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"

namespace HDData
{
	class Material;

	class HODO_API MeshRenderer : public RendererBase
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		void LoadMesh(const std::string& fileName);
		void LoadMaterial(HDData::Material* material, unsigned int element = 0);
		void LoadAlbedoMap(const std::string& fileName, unsigned int element = 0);
		void LoadNormalMap(const std::string& fileName, unsigned int element = 0);
		void LoadARMMap(const std::string& fileName, unsigned int element = 0);
		void LoadRoughnessMap(const std::string& fileName, unsigned int element = 0);
		void LoadMetallicMap(const std::string& fileName, unsigned int element = 0);
		void SetRoughnessValue(float value, unsigned int element = 0);
		void SetMetallicValue(float value, unsigned int element = 0);
		void SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element = 0);
		void SetUseLight(bool useLight);
		void SetMeshActive(bool isActive, unsigned int index);
		int GetMeshCount();

	protected:
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		HDEngine::IStaticMesh* _staticMesh;
	};
}
