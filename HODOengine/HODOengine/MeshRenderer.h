#pragma once
#include "Component.h"
#include <memory>

namespace HDEngine
{
	class IStaticMesh;
}

namespace HDData
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();

		virtual void Update() override;

		void SetActive(bool active);
		void LoadMesh(const char* fileName);
		void LoadDiffuseMap(const char* fileName);
		void LoadNormalMap(const char* fileName);

	private:
		std::unique_ptr<HDEngine::IStaticMesh> _staticMesh;
	};
}
