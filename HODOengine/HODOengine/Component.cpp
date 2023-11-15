#include "Component.h"
#include "GameObject.h"

hodoEngine::GameObject* hodoEngine::Component::GetGameObject() const
{
	return _gameObject;
}

hodoEngine::Transform* hodoEngine::Component::GetTransform() const
{
	return _gameObject->GetTransform();
}
