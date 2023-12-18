#pragma once
#include "IRenderable.h"

namespace HDEngine
{
	class IStaticMesh : public IRenderable
	{
	public:
		virtual ~IStaticMesh() {}
		virtual void LoadMesh(const char* fileName) = 0;
		virtual void LoadNormalMap(const char* fileName) = 0;
		virtual void LoadDiffuseMap(const char* fileName) = 0;
	};
}