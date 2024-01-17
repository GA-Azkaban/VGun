#include "GameObject.h"
#include "Transform.h"

namespace HDData
{
	GameObject::GameObject(std::string name /*= ""*/)
		:_objectName(name), _parentGameObject(nullptr), _selfActive(true)
	{
		_transform = AddComponent<Transform>();
	}

	void GameObject::OnEnable()
	{

	}

	void GameObject::OnDisable()
	{

	}

	void GameObject::Start()
	{
		if (!_selfActive)
			return;

		for (auto& component : _components)
		{
			if (component->_isStarted == false)
			{
				component->Start();
				component->_isStarted = true;
			}
		}
	}

	void GameObject::Update()
	{
		if (!_selfActive)
			return;

		for (auto& component : _components)
		{
			component->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		if (!_selfActive)
			return;

		for (auto& component : _components)
		{
			component->LateUpdate();
		}
	}

	void GameObject::FixedUpdate()
	{
		if (!_selfActive)
			return;

		for (auto& component : _components)
		{
			component->FixedUpdate();
		}
	}

	void GameObject::OnDestroy()
	{
		if (!_selfActive)
			return;

		for (auto& component : _components)
		{
			component->OnDestroy();
		}
	}

	void GameObject::OnCollisionEnter()
	{
		if (!_selfActive)
			return;

		for (auto& component : _components)
		{
			component->OnCollisionEnter();
		}
	}

	void GameObject::OnCollisionStay()
	{
		if (!_selfActive)
			return;

		for (auto& component : _components)
		{
			component->OnCollisionStay();
		}
	}

	void GameObject::OnCollisionExit()
	{
		if (!_selfActive)
			return;

		for (auto& component : _components)
		{
			component->OnCollisionExit();
		}
	}

	const std::unordered_set<Component*>& GameObject::GetAllComponents() const
	{
		return _components;
	}

	const std::unordered_set<GameObject*>& GameObject::GetChildGameObjects() const
	{
		return _childGameObjects;
	}

	void GameObject::SetParentObject(GameObject* parentObject)
	{
		_parentGameObject = parentObject;
		_parentGameObject->_childGameObjects.insert(this);
	}

	void GameObject::SetSelfActive(bool active)
	{
		_selfActive = active;

		for (const auto& comp : GetAllComponents())
		{
			comp->SetActive(active);
		}

		// 자식 오브젝트도 모두 활성화/비활성화
		for (auto child : _childGameObjects)
		{
			child->SetSelfActive(active);
		}
	}

	std::string GameObject::GetObjectName()
	{
		return _objectName;
	}
	
	void GameObject::OnUpdateTransform()
	{
		for (auto& component : _components)
		{
			component->OnUpdateTransform();
		}

		for (auto& child : _childGameObjects)
		{
			child->OnUpdateTransform();
		}
	}

}