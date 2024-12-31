#include "Light.h"
#include "GraphicsObjFactory.h"

namespace HDData
{

	Light::Light()
		: _light(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateLight())
	{

	}

	void Light::SetPosition(const Vector4& position)
	{
		//_light->SetPosition(position);
		_lightPosition = position;
	}

	void Light::SetDirection(const Vector4& direction)
	{
		//_light->SetDirection(direction);
		_lightDirection = direction;
	}

	void Light::SetColor(const Vector4& color)
	{
		//_light->SetColor(color);
		_lightColor = color;
	}

	void Light::SetLightType(LightType type)
	{
		//_light->SetLightType(static_cast<unsigned int>(type));
		_lightType = type;
	}

	void Light::SetActive(bool isActive)
	{
		//_light->SetActive(isActive);
		_isActive = isActive;
	}

	void Light::Update()
	{
		_light->SetDirection(_lightDirection);
		_light->SetColor(_lightColor);
	}

}
