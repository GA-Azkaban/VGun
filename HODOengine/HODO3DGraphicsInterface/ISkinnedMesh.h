#pragma once
#include <string>
#include <windows.h>
#include "IRenderable.h"
#include "IMaterial.h"

class Node;

namespace HDEngine
{
	class ISkinnedMesh : public IRenderable
	{
	public:
		virtual void LoadMesh(const std::string& fileName) = 0;
		virtual void LoadNode(const std::string& fileName) = 0;
		virtual void LoadMaterial(HDEngine::IMaterial* material, unsigned int element = 0) = 0;
		virtual void LoadAlbedoMap(const std::string& fileName, unsigned int element = 0) = 0;
		virtual void LoadNormalMap(const std::string& fileName, unsigned int element = 0) = 0;
		virtual void LoadARMMap(const std::string& fileName, unsigned int element = 0) = 0;
		virtual void LoadRoughnessMap(const std::string& fileName, unsigned int element = 0) = 0;
		virtual void LoadMetallicMap(const std::string& fileName, unsigned int element = 0) = 0;
		virtual void SetRoughnessValue(float value, unsigned int element = 0) = 0;
		virtual void SetMetallicValue(float value, unsigned int element = 0) = 0;
		virtual void SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element = 0) = 0;
		virtual void SetOutlineActive(bool isActive) = 0;
		virtual void PlayAnimation(const std::string& animName, bool isLoop = true, bool hasExitTime = false, float exitTime = 0.0f) = 0;
		virtual void PlayAnimationUpper(const std::string& animName, bool isLoop = true, bool hasExitTime = false, float exitTime = 0.0f) = 0;
		virtual void PlayAnimationLower(const std::string& animName, bool isLoop = true, bool hasExitTime = false, float exitTime = 0.0f) = 0;
		virtual void SetBlendDuration(float duration) = 0;
		virtual void SetBlendDurationUpper(float duration) = 0;
		virtual void SetBlendDurationLower(float duration) = 0;
		virtual bool IsAnimationEnd() = 0;
		virtual bool IsUpperAnimationEnd() = 0;
		virtual bool IsLowerAnimationEnd() = 0;
		virtual Node* GetNode() = 0;
	};
}
