#include "SkinnedMeshRenderer.h"
#include "RenderSystem.h"
#include "GraphicsObjFactory.h"
#include "Transform.h"
#include "GameObject.h"
#include "HDMaterial.h"

namespace HDData
{
	SkinnedMeshRenderer::SkinnedMeshRenderer()
		: _skinnedMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSkinnedMeshObject())
	{
		
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
		_skinnedMesh = nullptr;
	}

	void SkinnedMeshRenderer::LoadMesh(const std::string& fileName)
	{
		_skinnedMesh->LoadMesh(fileName);
	}

	void SkinnedMeshRenderer::LoadNode(const std::string& fileName)
	{
		_skinnedMesh->LoadNode(fileName);
	}
	
	void SkinnedMeshRenderer::LoadMaterial(HDData::Material* material, unsigned int element /*= 0*/)
	{
		_skinnedMesh->LoadMaterial(material->_material, element);
	}

	void SkinnedMeshRenderer::LoadNormalMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_skinnedMesh->LoadNormalMap(fileName, element);
	}

	void SkinnedMeshRenderer::LoadAlbedoMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_skinnedMesh->LoadAlbedoMap(fileName, element);
	}

	void SkinnedMeshRenderer::LoadARMMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
        _skinnedMesh->LoadARMMap(fileName, element);
    }

	void SkinnedMeshRenderer::LoadRoughnessMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_skinnedMesh->LoadRoughnessMap(fileName, element);
	}

	void SkinnedMeshRenderer::LoadMetallicMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_skinnedMesh->LoadMetallicMap(fileName, element);
	}

	void SkinnedMeshRenderer::SetRoughnessValue(float value, unsigned int element /* = 0 */)
	{
		_skinnedMesh->SetRoughnessValue(value, element);
	}

	void SkinnedMeshRenderer::SetMetallicValue(float value, unsigned int element /* = 0 */)
	{
		_skinnedMesh->SetMetallicValue(value, element);
	}

	void SkinnedMeshRenderer::SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element /* = 0 */)
	{
		_skinnedMesh->SetAlbedoColor(r, g, b, a, element);
	}

	void SkinnedMeshRenderer::PlayAnimation(const std::string& animName, bool isLoop /*= true*/)
	{
		_skinnedMesh->PlayAnimation(animName, isLoop);
	}

	void SkinnedMeshRenderer::PlayAnimationUpper(const std::string& animName, bool isLoop /*= true*/)
	{
		_skinnedMesh->PlayAnimationUpper(animName, isLoop);
	}

	void SkinnedMeshRenderer::PlayAnimationLower(const std::string& animName, bool isLoop /*= true*/)
	{
		_skinnedMesh->PlayAnimationLower(animName, isLoop);
	}

	void SkinnedMeshRenderer::SetBlendDuration(float duration)
	{
		_skinnedMesh->SetBlendDuration(duration);
	}

	bool SkinnedMeshRenderer::IsAnimationEnd()
	{
		return _skinnedMesh->IsAnimationEnd();
	}

	bool SkinnedMeshRenderer::IsUpperAnimationEnd()
	{
		return _skinnedMesh->IsUpperAnimationEnd();
	}

	bool SkinnedMeshRenderer::IsLowerAnimationEnd()
	{
		return _skinnedMesh->IsLowerAnimationEnd();
	}

	void SkinnedMeshRenderer::SetOutlineActive(bool isActive)
	{
		_skinnedMesh->SetOutlineActive(isActive);
	}

	Node* SkinnedMeshRenderer::GetNode()
	{
		return _skinnedMesh->GetNode();
	}

	void SkinnedMeshRenderer::Update()
	{
		_skinnedMesh->SetWorldTM(GetTransform()->GetWorldTM());
	}

	void SkinnedMeshRenderer::OnEnable()
	{
		_skinnedMesh->SetActive(true);
	}

	void SkinnedMeshRenderer::OnDisable()
	{
		_skinnedMesh->SetActive(false);
	}

}
