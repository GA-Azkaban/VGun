#pragma once
#include "RendererBase.h"
#include "dllExporter.h"
#include "..\\HODO3DGraphicsInterface\\IStaticMesh.h"

namespace HDData
{
	class HODO_API MeshRenderer : public RendererBase
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		void LoadMesh(const std::string& fileName);
		void LoadNormalMap(const std::string& fileName);
		void LoadDiffuseMap(const std::string& fileName);
		void LoadARMMap(const std::string& fileName);
		void LoadRoughnessMap(const std::string& fileName);
		void LoadMetallicMap(const std::string& fileName);
		void SetRoughnessValue(float value);
		void SetMetallicValue(float value);

	protected:
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		HDEngine::IStaticMesh* _staticMesh;
	};
}
