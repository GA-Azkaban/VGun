#pragma once
#include "../HODO3DGraphicsInterface/ISkinnedMesh.h"
#include "SkinningMeshObject.h"
#include "../HODOmath/HODOmath.h"
#include <memory>

namespace hodoGIAdapter
{
	class SkinnedMeshAdapter : public HDEngine::ISkinnedMesh
	{
	public:
		SkinnedMeshAdapter(SkinningMeshObject* skinningMeshObject)
			: _skinningMeshObject(skinningMeshObject)
		{

		}

		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& worldTM) override;
		virtual void SetActive(bool isActive) override;

		virtual void LoadMesh(const std::string& fileName) override;
		virtual void LoadNormalMap(const std::string& fileName) override;
		virtual void LoadDiffuseMap(const std::string& fileName) override;
		//virtual void LoadVertexShader(const std::string& fileName) override;
		//virtual void LoadPixelShader(const std::string& fileName) override;
		virtual void PlayAnimation(const std::string& animName, bool isLoop = true) override;
		virtual void PlayAnimation(UINT index, bool isLoop = true) override;

	private:
		std::unique_ptr<SkinningMeshObject> _skinningMeshObject;
	};
}