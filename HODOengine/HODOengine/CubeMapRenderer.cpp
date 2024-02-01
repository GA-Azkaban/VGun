#include "CubeMapRenderer.h"
#include "GraphicsObjFactory.h"

namespace HDData
{

	CubeMapRenderer::CubeMapRenderer()
		: _cubeMap(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCubeMap())
	{

	}

	void CubeMapRenderer::LoadCubeMapTexture(const std::string& fileName)
	{
		_cubeMap->LoadCubeMapTexture(fileName);
	}

	void CubeMapRenderer::OnEnable()
	{
		_cubeMap->SetActive(true);
	}

	void CubeMapRenderer::OnDisable()
	{
		_cubeMap->SetActive(false);
	}

}
