#include "GameObject.h"
#include "Transform.h"
using namespace hodoEngine;

hodoEngine::GameObject::GameObject(GameObject* parent)
{
	transform = AddComponent<Transform>();
	parentGameObject = parent;
	parent->childrenGameObject.push_back(this);
}

void hodoEngine::GameObject::DeleteComponent(Component* component)
{
	for (auto iter = components.begin(); iter != components.end(); ++iter)
	{
		if (*iter == component)
		{
			components.erase(*iter);
			break;
		}
	}
}
