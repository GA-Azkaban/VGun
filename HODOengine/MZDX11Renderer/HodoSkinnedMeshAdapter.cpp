#include "HodoSkinnedMeshAdapter.h"
#include "ResourceManager.h"
#include <DirectXMath.h>

namespace hodoGIAdapter
{
	void SkinnedMeshAdapter::SetWorldTM(const HDMath::HDFLOAT4X4& worldTM)
	{
		DirectX::XMMATRIX tm;

		tm.r[0].m128_f32[0] = worldTM._11;
		tm.r[0].m128_f32[1] = worldTM._12;
		tm.r[0].m128_f32[2] = worldTM._13;
		tm.r[0].m128_f32[3] = worldTM._14;

		tm.r[1].m128_f32[0] = worldTM._21;
		tm.r[1].m128_f32[1] = worldTM._22;
		tm.r[1].m128_f32[2] = worldTM._23;
		tm.r[1].m128_f32[3] = worldTM._24;

		tm.r[2].m128_f32[0] = worldTM._31;
		tm.r[2].m128_f32[1] = worldTM._32;
		tm.r[2].m128_f32[2] = worldTM._33;
		tm.r[2].m128_f32[3] = worldTM._34;

		tm.r[3].m128_f32[0] = worldTM._41;
		tm.r[3].m128_f32[1] = worldTM._42;
		tm.r[3].m128_f32[2] = worldTM._43;
		tm.r[3].m128_f32[3] = worldTM._44;

		_skinningMeshObject->SetWorldTM(tm);
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