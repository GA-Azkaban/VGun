#pragma once
#include "IRenderable.h"

namespace hodoGI
{
	class ISkinnedMesh : public IRenderable
	{
	public:
		virtual void LoadMesh(const char* fileName) = 0;
		virtual void LoadNormalMap(const char* fileName) = 0;
		virtual void LoadDiffuseMap(const char* fileName) = 0;
		virtual void SetAnimation(const char* animationName) = 0;
		virtual void PlayAnimation() = 0;
	};
}