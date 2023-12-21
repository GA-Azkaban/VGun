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
		HDEngine::IStaticMesh& Get();

	private:
		HDEngine::IStaticMesh* _staticMesh;
	};
}
