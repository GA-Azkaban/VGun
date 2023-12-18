#pragma once
#include "RendererBase.h"
#include "..\\HODO3DGraphicsInterface\\IStaticMesh.h"
#include <memory>

namespace HDData
{
	class MeshRenderer : public RendererBase
	{
	public:
		MeshRenderer();
		HDEngine::IStaticMesh& Get();

	private:
		std::unique_ptr<HDEngine::IStaticMesh> _staticMesh;
	};
}
