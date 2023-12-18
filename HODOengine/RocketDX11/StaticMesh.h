#pragma once
#include "..\\HODO3DGraphicsInterface\\IRenderable.h"

namespace RocketCore::Graphics
{
	class StaticMesh : public HDEngine::IRenderable
	{
	public:
		virtual void SetMesh(eMeshList mesh) override;
		virtual void SetMesh(const std::string& modelPath) override;
		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& worldTM) override;
		virtual void SetActive(bool isActive) override;
	};
}
	