#include "GameObject.h"
#include "Transform.h"
using namespace hodoEngine;

hodoEngine::GameObject::GameObject(GameObject* parent)
{
	_transform = AddComponent<Transform>();
	_parentGameObject = parent;
	parent->_childrenGameObjects.push_back(this);
}

void hodoEngine::GameObject::DeleteComponent(Component* component)
{
	for (auto iter = _components.begin(); iter != _components.end(); ++iter)
	{
		if (*iter == component)
		{
			_components.erase(*iter);
			break;
		}
	}
}

<<<<<<< HEAD
GameObject::GameObject(std::string name)
{
	objName = name;
=======
void GameObject::SetSelfActive(bool active)
{
	_selfActive = active;
	// �ڽ� ������Ʈ�� ��� Ȱ��ȭ/��Ȱ��ȭ
	for (int i = 0; i < _childrenGameObjects.size(); ++i)
	{
		_childrenGameObjects[i]->SetSelfActive(active);
	}

>>>>>>> MJKIM-GameObjectAndComponent
}
