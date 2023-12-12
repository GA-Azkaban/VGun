#include "Component.h"
#include "GameObject.h"

HDData::GameObject* HDData::Component::GetGameObject() const
{
	return _gameObject;
}

HDData::Transform* HDData::Component::GetTransform() const
{
	return _gameObject->GetTransform();
}
