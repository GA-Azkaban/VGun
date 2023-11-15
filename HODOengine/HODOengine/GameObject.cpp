#include "GameObject.h"
#include "Transform.h"
using namespace hodoEngine;

GameObject::GameObject(std::string name /*= ""*/)
	:_objectName(name), _parentGameObject(nullptr), _selfActive(true)
{
	_transform = AddComponent<Transform>();
}

//hodoEngine::GameObject::GameObject(GameObject* parent)
//{
//	_transform = AddComponent<Transform>();
//}

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

void GameObject::SetParentObject(GameObject* parentObject)
{

}

GameObject::GameObject()
{
	_transform = AddComponent<Transform>();
}

void GameObject::SetSelfActive(bool active)
{
	_selfActive = active;
	// �ڽ� ������Ʈ�� ��� Ȱ��ȭ/��Ȱ��ȭ
	for (int i = 0; i < _childrenGameObjects.size(); ++i)
	{
		_childrenGameObjects[i]->SetSelfActive(active);
	}
}
