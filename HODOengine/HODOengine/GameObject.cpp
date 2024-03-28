#include "GameObject.h"
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
	}

	void GameObject::FlushDisable()
	{
		_selfActive = false;

		for (auto& component : _components)
		{
			component->OnDisable();
		}
	}

	void GameObject::Start()
	{
		if (!GetParentActive()) return;

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
		if (!GetParentActive()) return;

		for (auto& component : _components)
		{
			component->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		if (!GetParentActive()) return;

		for (auto& component : _components)
		{
			component->LateUpdate();
		}
	}

	void GameObject::FixedUpdate()
	{
		if (!GetParentActive()) return;

		for (auto& component : _components)
		{
			component->FixedUpdate();
		}
	}

	void GameObject::OnDestroy()
	{
		if (!GetParentActive()) return;

		for (auto& component : _components)
		{
			component->OnDestroy();
		}
	}

	void GameObject::OnCollisionEnter()
	{
		if (!GetParentActive()) return;

		for (auto& component : _components)
		{
			component->OnCollisionEnter();
		}
	}

	void GameObject::OnCollisionStay()
	{
		if (!GetParentActive()) return;

		for (auto& component : _components)
		{
			component->OnCollisionStay();
		}
	}

	void GameObject::OnCollisionExit()
	{
		if (!GetParentActive()) return;

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

	void GameObject::FadeOut()
	{
		// 매번 검은색 이미지를 만들고 호출해야하나?
	}

	void GameObject::FadeIn()
	{

	}

}
