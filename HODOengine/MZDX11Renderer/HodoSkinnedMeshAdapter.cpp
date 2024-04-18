#include "HodoSkinnedMeshAdapter.h"
#include "ResourceManager.h"
#include <DirectXMath.h>

namespace hodoGIAdapter
{
	void SkinnedMeshAdapter::SetWorldTM(const Matrix& worldTM)
	{
		_skinningMeshObject->SetWorldTM(worldTM);
	}

	void SkinnedMeshAdapter::SetActive(bool isActive)
	{
		_skinningMeshObject->SetActive(isActive);
	}

	void SkinnedMeshAdapter::LoadMesh(const std::string& fileName)
	{
		_skinningMeshObject->SetMesh(fileName);
	}

	void SkinnedMeshAdapter::LoadNormalMap(const std::string& fileName)
	{
		_skinningMeshObject->SetNormalTexture(fileName);
	}

	void SkinnedMeshAdapter::LoadDiffuseMap(const std::string& fileName)
	{
		_skinningMeshObject->SetDiffuseTexture(fileName);
	}

	/*void SkinnedMeshAdapter::LoadVertexShader(const std::string& fileName)
	{
		_skinningMeshObject->SetVertexShader(fileName);
	}

	void SkinnedMeshAdapter::LoadPixelShader(const std::string& fileName)
	{
		_skinningMeshObject->SetPixelShader(fileName);
	}*/

	void SkinnedMeshAdapter::PlayAnimation(const std::string& animName, bool isLoop /*= true*/)
	{
		_skinningMeshObject->PlayAnimation(animName, isLoop);
	}

	void SkinnedMeshAdapter::PlayAnimation(UINT index, bool isLoop /*= true*/)
	{
		_skinningMeshObject->PlayAnimation(index, isLoop);
	}
}