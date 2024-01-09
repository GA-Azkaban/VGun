#include "HodoStaticMeshAdapter.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include <DirectXMath.h>

namespace hodoGIAdapter
{
	void StaticMeshAdapter::SetWorldTM(const HDMath::HDFLOAT4X4& worldTM)
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

		_staticMeshObject->SetWorldTM(tm);
	}

	void StaticMeshAdapter::SetActive(bool isActive)
	{
		_staticMeshObject->SetActive(isActive);
	}

	void StaticMeshAdapter::LoadMesh(const std::string& fileName)
	{
		_staticMeshObject->SetMesh(fileName);
	}

	void StaticMeshAdapter::LoadNormalMap(const std::string& fileName)
	{
		_staticMeshObject->SetNormalTexture(fileName);
	}

	void StaticMeshAdapter::LoadDiffuseMap(const std::string& fileName)
	{
		_staticMeshObject->SetDiffuseTexture(fileName);
	}

	void StaticMeshAdapter::LoadVertexShader(const std::string& fileName)
	{
		_staticMeshObject->SetVertexShader(fileName);
	}

	void StaticMeshAdapter::LoadPixelShader(const std::string& fileName)
	{
		_staticMeshObject->SetPixelShader(fileName);
	}
}