﻿#include "GameObject.h"
#include "Transform.h"
#include "ObjectSystem.h"

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

		for (auto& component : _components)
		{
			component->OnEnable();
		}

		for (auto& obj : _childGameObjects)
		{
			obj->FlushEnable();
		}
	}

	void GameObject::FlushDisable()
	{
		_selfActive = false;

		for (auto& component : _components)
		{
			component->OnDisable();
		}

		for (auto& obj : _childGameObjects)
		{
			obj->FlushDisable();
		}
	}

	void GameObject::Start()
	{
		if (!_selfActive)
			return;

		for (auto& component : _components)
		{
			if (!component->_isStarted)
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
		// clear parent
		if (parentObject == nullptr)
		{
			if (_parentGameObject != nullptr)
			{
				std::erase_if(_parentGameObject->_childGameObjects, [this](GameObject* obj) {return obj == this; });
				_parentGameObject = nullptr;
			}
		}
		else
		{
			_parentGameObject = parentObject;
			_parentGameObject->_childGameObjects.insert(this);
		}
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

	bool GameObject::GetSelfActive()
	{
		return _selfActive;
	}

	std::string GameObject::GetObjectName()
	{
		return _objectName;
	}

}
