#include "GameObject.h"
#include "Transform.h"

namespace HDData
{
	GameObject::GameObject(std::string name /*= ""*/)
		:_objectName(name), _parentGameObject(nullptr), _selfActive(true)
	{
		_transform = AddComponent<Transform>();
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
}