#pragma once
#include <string>
#include <windows.h>
#include "IRenderable.h"

namespace HDEngine
{
	class ISkinnedMesh : public IRenderable
	{
	public:
		virtual void LoadMesh(const std::string& fileName) = 0;
		virtual void LoadNormalMap(const std::string& fileName) = 0;
		virtual void LoadAlbedoMap(const std::string& fileName) = 0;
		virtual void LoadARMMap(const std::string& fileName) = 0;
		virtual void PlayAnimation(const std::string& fileName, bool isLoop = true) = 0;
		virtual bool IsAnimationEnd() = 0;
		virtual void SetOutlineActive(bool isActive) = 0;
		virtual const Matrix& GetBoneTransformByNodeName(std::string nodeName) = 0;
		virtual void LoadRoughnessMap(const std::string& fileName) = 0;
		virtual void LoadMetallicMap(const std::string& fileName) = 0;
		virtual void SetRoughnessValue(float value) = 0;
		virtual void SetMetallicValue(float value) = 0;
		virtual void SetAlbedoColor(UINT r, UINT g, UINT b, UINT a = 255) = 0;
		virtual void SetAlbedoColor(Vector4 color) = 0;
	};
}
