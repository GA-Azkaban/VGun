#include "Component.h"
#include "GameObject.h"

hodoData::GameObject* hodoData::Component::GetGameObject() const
{
	return _gameObject;
}

hodoData::Transform* hodoData::Component::GetTransform() const
{
	return _gameObject->GetTransform();
}
