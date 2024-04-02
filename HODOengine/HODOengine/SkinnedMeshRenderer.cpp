#include "SkinnedMeshRenderer.h"
#include "RenderSystem.h"
#include "GraphicsObjFactory.h"
#include "Transform.h"
#include "GameObject.h"

namespace HDData
{
	SkinnedMeshRenderer::SkinnedMeshRenderer()
		: _skinnedMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSkinnedMeshObject())
	{
		//HDEngine::RenderSystem::Instance().PushRenderComponent(this);
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

	void SkinnedMeshRenderer::LoadNormalMap(const std::string& fileName)
	{
		_skinnedMesh->LoadNormalMap(fileName);
	}

	void SkinnedMeshRenderer::LoadAlbedoMap(const std::string& fileName)
	{
		_skinnedMesh->LoadAlbedoMap(fileName);
	}

	void SkinnedMeshRenderer::LoadARMMap(const std::string& fileName)
	{
        _skinnedMesh->LoadARMMap(fileName);
    }

	void SkinnedMeshRenderer::LoadRoughnessMap(const std::string& fileName)
	{
		_skinnedMesh->LoadRoughnessMap(fileName);
	}

	void SkinnedMeshRenderer::LoadMetallicMap(const std::string& fileName)
	{
		_skinnedMesh->LoadMetallicMap(fileName);
	}

	void SkinnedMeshRenderer::SetRoughnessValue(float value)
	{
		_skinnedMesh->SetRoughnessValue(value);
	}

	void SkinnedMeshRenderer::SetMetallicValue(float value)
	{
		_skinnedMesh->SetMetallicValue(value);
	}

	void SkinnedMeshRenderer::SetAlbedoColor(UINT r, UINT g, UINT b, UINT a /*= 255*/)
	{
		_skinnedMesh->SetAlbedoColor(r, g, b, a);
	}

	void SkinnedMeshRenderer::SetAlbedoColor(Vector4 color)
	{
		_skinnedMesh->SetAlbedoColor(color);
	}

	void SkinnedMeshRenderer::PlayAnimation(const std::string& animName, bool isLoop /*= true*/)
	{
		_skinnedMesh->PlayAnimation(animName, isLoop);
	}

	bool SkinnedMeshRenderer::IsAnimationEnd()
	{
		return _skinnedMesh->IsAnimationEnd();
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
		// 여기에서 노드 오브젝트 갱신
		// 렌더러의 오브젝트에서 타고 올라가서 가장 위에 있는 부모 오브젝트를 가져온다. 트랜스폼 갱신
		// 재귀함수로 부모 오브젝트 밑에 있는 자식 오브젝트들의 트랜스폼 갱신
		//UpdateArmatureNodeTransform(_skinnedMesh->GetNode(), GetGameObject()->GetParentGameObject()->GetChildGameObjectByName("Armature"));
		Node* node = _skinnedMesh->GetNode();
	}

	void SkinnedMeshRenderer::OnEnable()
	{
		_skinnedMesh->SetActive(true);
	}

	void SkinnedMeshRenderer::OnDisable()
	{
		_skinnedMesh->SetActive(false);
	}

	void SkinnedMeshRenderer::UpdateArmatureNodeTransform(Node* node, GameObject* parentObject)
	{
		//for (auto& child : parentObject->GetChildGameObjects())
		//{
		//	child->GetTransform()->SetLocalTM(node->nodeTransform);
		//	UpdateArmatureNodeTransform(node->children)
		//}
	}

}
