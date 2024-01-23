#pragma once
#include "RendererBase.h"
#include "dllExporter.h"
#include "../HODO3DGraphicsInterface/ISkinnedMesh.h"

namespace HDData
{
	class HODO_API SkinnedMeshRenderer : public RendererBase
	{
	public:
		SkinnedMeshRenderer();
		HDEngine::ISkinnedMesh& Get();

	private:
		HDEngine::ISkinnedMesh* _skinnedMesh;
	};
}

