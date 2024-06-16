#include "GameObject.h"
#include "Transform.h"
#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "DynamicBoxCollider.h"
#include "StaticBoxCollider.h"
#include "SkinnedMeshRenderer.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "SceneSystem.h"

#include <algorithm>

namespace HDEngine
{
	class SceneSystem;
}

namespace HDData
{
	GameObject::GameObject(std::string name /*= ""*/)
		:_objectName(name), _parentGameObject(nullptr), _selfActive(true)
	{
		_transform = AddComponent<Transform>();
	}

	void GameObject::OnEnable()
	{
		HDEngine::ObjectSystem::Instance().AddOnEnableList(this);
	}

	void GameObject::OnDisable()
	{
		HDEngine::ObjectSystem::Instance().AddOnDisableList(this);
	}

	void GameObject::FlushEnable()
	{
		_selfActive = true;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			_componentsIndexed[i]->OnEnable();
		}
	}

	void GameObject::FlushDisable()
	{
		_selfActive = false;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			_componentsIndexed[i]->OnDisable();
		}
	}

	void GameObject::Start()
	{
		if (!GetParentActive()) return;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			if (!_componentsIndexed[i]->_isStarted)
			{
				_componentsIndexed[i]->Start();
				_componentsIndexed[i]->_isStarted = true;
			}
		}
	}

	void GameObject::Update()
	{
		if (!GetParentActive()) return;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			if(!_componentsIndexed[i]->_isStarted) continue;
			else { _componentsIndexed[i]->Update(); }
		}
	}

	void GameObject::LateUpdate()
	{
		if (!GetParentActive()) return;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			_componentsIndexed[i]->LateUpdate();
		}
	}

	void GameObject::FixedUpdate()
	{
		if (!GetParentActive()) return;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			_componentsIndexed[i]->FixedUpdate();
		}
	}

	void GameObject::OnDestroy()
	{
		if (!GetParentActive()) return;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			_componentsIndexed[i]->OnDestroy();
		}
	}

	void GameObject::OnCollisionEnter(PhysicsCollision** _colArr, unsigned int count)
{
		if (!_selfActive)
		{
			return;
		}

		std::for_each(_componentsIndexed.begin(), _componentsIndexed.end(), 
			[&_colArr, &count](auto& iter){ iter->OnCollisionEnter(_colArr, count); });
	}

	void GameObject::OnCollisionStay()
	{
		if (!GetParentActive()) return;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			_componentsIndexed[i]->OnCollisionStay();
		}
	}

	void GameObject::OnCollisionExit(PhysicsCollision** _colArr, unsigned int count)
{
		if (!_selfActive)
		{
			return;
		}

		std::for_each(_componentsIndexed.begin(), _componentsIndexed.end(),
			[&_colArr, &count](auto& iter) { iter->OnCollisionExit(_colArr, count); });
	}

	const std::vector<Component*>& GameObject::GetAllComponents() const
	{
		return _componentsIndexed;
	}

	const std::vector<GameObject*>& GameObject::GetChildGameObjects() const
	{
		return _childGameObjectsIndexed;
	}

	GameObject* GameObject::GetGameObjectByNameInChildren(const std::string& objectName)
	{
		if (_objectName == objectName)
			return this;

		for (int i = 0; i < _childGameObjectsIndexed.size(); ++i)
		{		
			GameObject* founded = _childGameObjectsIndexed[i]->GetGameObjectByNameInChildren(objectName);
			if (founded != nullptr)
				return founded;
		}
		
		return nullptr;
	}

	Scene* GameObject::GetThisObjectScene()
	{
		for (auto& scene : HDEngine::SceneSystem::Instance().GetAllScenes())
		{
			for (auto& obj : scene.second->GetGameObjectList())
			{
				if (obj == this) return scene.second;
			}
		}
		return nullptr;
	}

	void GameObject::SetParentObject(GameObject* parentObject)
	{
		// clear parent
		if (parentObject == nullptr)
		{
			if (_parentGameObject != nullptr)
			{
				for (auto iter = _parentGameObject->_childGameObjectsIndexed.begin(); iter != _parentGameObject->_childGameObjectsIndexed.end(); ++iter)
				{
					if (*iter == this)
					{
						_parentGameObject->_childGameObjectsIndexed.erase(iter);
					}
				}
				_parentGameObject = nullptr;
			}
		}
		else // Set parent
		{
			_parentGameObject = parentObject;
			_parentGameObject->_childGameObjectsIndexed.push_back(this);
		}
	}

	void GameObject::SetSelfActive(bool active)
	{
		_selfActive = active;

		for (const auto& comp : GetAllComponents())
		{
			comp->SetActive(active);
		}

		_selfActive = active;
	}

	bool GameObject::GetSelfActive()
	{
		return _selfActive;
	}

	bool GameObject::GetParentActive()
	{
		if (GetParentGameObject())
		{
			if (!GetParentGameObject()->GetSelfActive())
			{
				return false;
			}
			else
			{
				return GetParentGameObject()->GetParentActive();
			}
		}
		else
		{
			return GetSelfActive();
		}
	}

	void GameObject::SetObjectName(std::string str)
	{
		_objectName = str;
	}

	std::string GameObject::GetObjectName()
	{
		return _objectName;
	}

	GameObject* GameObject::GetParentObject()
	{
		return _parentGameObject;
	}

	void GameObject::LoadFBXFile(std::string fileName)
	{
		Scene* currentScene = GetThisObjectScene();
		GameObject* rendererObject = HDEngine::ObjectSystem::Instance().CreateObject(currentScene, "mesh", this);
		rendererObject->GetTransform()->Rotate(0.0f, 180.0f, 0.0f);
		auto rendererComp = rendererObject->AddComponent<SkinnedMeshRenderer>();
		rendererComp->LoadNode(fileName);
		rendererComp->LoadMesh(fileName);
		Node* rendererNode = rendererComp->GetNode();
		if (!rendererNode)
			return;

		Node* armature = FindNodeByName(rendererNode, "Armature");
		if (armature != nullptr)
		{
			ProcessNode(currentScene, armature, this);
		}
		else
		{
			GameObject* newObject = HDEngine::ObjectSystem::Instance().CreateObject(currentScene, "Armature", this);
			newObject->GetTransform()->SetLocalTM(rendererNode->rootNodeInvTransform);
			newObject->GetTransform()->Rotate(0.0f, 0.0f, 180.0f);
			Node* root = FindNodeByName(rendererNode, "root");			
			if (root != nullptr)
			{
				ProcessNode(currentScene, root, newObject);
			}
		}
		// 그래픽스엔진의 SkinningObject의 LoadMesh에서 호출되는 LoadNode는 없애야 한다.
		// 가져온 노드정보의 트랜스폼을 게임 오브젝트가 가진 트랜스폼과 연결시켜야 한다.
	}

	Node* GameObject::FindNodeByName(Node* node, std::string nodeName)
	{
		if (node->name == nodeName)
		{
			return node;
		}

		for (int i = 0; i < node->children.size(); ++i)
		{
			Node* findNode = FindNodeByName(&(node->children[i]), nodeName);
			if (findNode)
				return findNode;
		}

		return nullptr;
	}

	void GameObject::ProcessNode(Scene* scene, Node* node, GameObject* parentObject)
	{
		GameObject* newObject = HDEngine::ObjectSystem::Instance().CreateObject(scene, node->name, parentObject);
		newObject->GetTransform()->SetLocalTM(node->nodeTransformOffset);
		node->nodeTransform = newObject->GetTransform()->_nodeTransform;
		newObject->AddComponent<StaticBoxCollider>();
		
		for (int i = 0; i < node->children.size(); ++i)
		{
			ProcessNode(scene, &(node->children[i]), newObject);
		}
	}
	
}
