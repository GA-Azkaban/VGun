#pragma once
#include <string>
#include "eMeshList.h"

namespace HDMath
{
	struct HDFLOAT4X4;
}

namespace HDEngine
{
	class IRenderable
	{
	public:
		virtual ~IRenderable() {};
		virtual void SetMesh(eMeshList mesh) = 0;
		virtual void SetMesh(const std::string& modelPath) = 0;
		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& worldTM) = 0;
		virtual void SetActive(bool isActive) = 0;
	};
}