#include "MeshRenderer.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"

namespace HDData
{
	MeshRenderer::MeshRenderer()
		: _staticMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateStaticMeshObject())
	{
		HDEngine::RenderSystem::Instance().PushRenderComponent(this);
		_renderable = _staticMesh;
	}

	void MeshRenderer::LoadMesh(const std::string& fileName)
	{
		_staticMesh->LoadMesh(fileName);
	}

	void MeshRenderer::LoadNormalMap(const std::string& fileName)
	{
		_staticMesh->LoadNormalMap(fileName);
	}

	void MeshRenderer::LoadDiffuseMap(const std::string& fileName)
	{
		_staticMesh->LoadDiffuseMap(fileName);
	}

	void MeshRenderer::LoadVertexShader(const std::string& fileName)
	{
		_staticMesh->LoadVertexShader(fileName);
	}

	void MeshRenderer::LoadPixelShader(const std::string& fileName)
	{
		_staticMesh->LoadPixelShader(fileName);
	}
}