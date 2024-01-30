#include "HodoStaticMeshAdapter.h"
#include "ResourceManager.h"
#include <DirectXMath.h>

namespace hodoGIAdapter
{
	void StaticMeshAdapter::SetWorldTM(const Matrix& worldTM)
	{		
		_staticMeshObject->SetWorldTM(worldTM);
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

	/*void StaticMeshAdapter::LoadVertexShader(const std::string& fileName)
	{
		_staticMeshObject->SetVertexShader(fileName);
	}

	void StaticMeshAdapter::LoadPixelShader(const std::string& fileName)
	{
		_staticMeshObject->SetPixelShader(fileName);
	}*/
}