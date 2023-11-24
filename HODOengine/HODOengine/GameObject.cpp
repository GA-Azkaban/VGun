#include "GameObject.h"
#include "Transform.h"

namespace hodoData
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

void GameObject::SetParentObject(GameObject* parentObject)
{
	_parentGameObject = parentObject;
	_parentGameObject->_childGameObjects.insert(this);
}

	_selfActive = active;

	// �ڽ� ������Ʈ�� ��� Ȱ��ȭ/��Ȱ��ȭ
	for (auto child : _childGameObjects)
	{
		child->SetSelfActive(active);
	}
}
}