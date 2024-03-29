#include "GameObject.h"
#include "Transform.h"
#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "HDResourceManager.h"
#include "DynamicBoxCollider.h"
#include "StaticBoxCollider.h"
#include "SkinnedMeshRenderer.h"

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
			_componentsIndexed[i]->Update();
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

	void GameObject::OnCollisionEnter()
	{
		if (!GetParentActive()) return;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			_componentsIndexed[i]->OnCollisionEnter();
		}
	}

	void GameObject::OnCollisionStay()
	{
		if (!GetParentActive()) return;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			_componentsIndexed[i]->OnCollisionStay();
		}
	}

	void GameObject::OnCollisionExit()
	{
		if (!GetParentActive()) return;

		for (int i = 0; i < _componentsIndexed.size(); ++i)
		{
			_componentsIndexed[i]->OnCollisionExit();
		}
	}

	const std::vector<Component*>& GameObject::GetAllComponents() const
	{
		return _componentsIndexed;
	}

	const std::vector<GameObject*>& GameObject::GetChildGameObjects() const
	{
		return _childGameObjectsIndexed;
	}

	GameObject* GameObject::GetChildGameObjectByName(const std::string& objectName)
	{
		GameObject* ret = nullptr;
		FindGameObjectByNameInChildren(_parentGameObject, objectName, ret);
		return ret;
	}

	void GameObject::SetParentObject(GameObject* parentObject)
	{
		// clear parent
		if (parentObject == nullptr)
		{
			if (_parentGameObject != nullptr)
			{
				for(auto iter = _parentGameObject->_childGameObjectsIndexed.begin(); iter != _parentGameObject->_childGameObjectsIndexed.end(); ++iter)
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

	std::string GameObject::GetObjectName()
	{
		return _objectName;
	}

	void GameObject::LoadNodeHierarchyFromFile(std::string fileName)
	{
		Node* rootNode = HDEngine::ResourceManager::Instance().GetNode(fileName);
		if (HDEngine::ResourceManager::Instance().GetNode(fileName) == nullptr)
			return;

		Node armature;
		FindNodeByName(rootNode, armature, "Armature");
		if (armature.name == "Armature")
		{
			ProcessNode(&armature, this);
		}


		GameObject* rendererObject = HDEngine::ObjectSystem::Instance().CreateObject(HDEngine::SceneSystem::Instance().GetCurrentScene(), "mesh", this);
		rendererObject->AddComponent<SkinnedMeshRenderer>();
	}

	void GameObject::FindNodeByName(Node* node, Node& outNode, std::string nodeName)
	{		
		if (node->name == nodeName)
		{
			outNode = *node;
			return;
		}

		for (int i = 0; i < node->children.size(); ++i)
		{
			FindNodeByName(&(node->children[i]), outNode, nodeName);
		}
	}

	void GameObject::ProcessNode(Node* node, GameObject* parentObject)
	{
		GameObject* newObject = HDEngine::ObjectSystem::Instance().CreateObject(HDEngine::SceneSystem::Instance().GetCurrentScene(), node->name, parentObject);
		newObject->AddComponent<StaticBoxCollider>();
		newObject->GetTransform()->SetLocalTM(node->nodeTransformOffset);
		for (int i = 0; i < node->children.size(); ++i)
		{
			ProcessNode(&(node->children[i]), newObject);
		}
	}

	void GameObject::FindGameObjectByNameInChildren(GameObject* parentObject, const std::string& name, GameObject* outGameObject)
	{
		for (auto& child : parentObject->GetChildGameObjects())
		{
			if (child->GetObjectName() == name)
			{
				outGameObject = child;
				return;
			}
			else
			{
				FindGameObjectByNameInChildren(child, name, outGameObject);
				return;
			}
		}
	}

}
