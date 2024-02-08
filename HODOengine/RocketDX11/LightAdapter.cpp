#include "LightAdapter.h"

namespace RocketCore::Graphics
{

	LightAdapter::LightAdapter()
	{
		light = LightManager::Instance().AddLight();
	}

	void LightAdapter::SetPosition(const Vector4& position)
	{
		light->position = position;
	}

	void LightAdapter::SetDirection(const Vector4& direction)
	{
		light->direction = direction;
	}

	void LightAdapter::SetColor(const Vector4& color)
	{
		light->color = color;
	}

	void LightAdapter::SetLightType(unsigned int type)
	{
		light->lightType = type;
	}

	void LightAdapter::SetActive(bool isActive)
	{
		if (light->status == (int)LightStatus::Static_Shadows)
		{
			return;
		}
		
		if (isActive == true)
		{
			light->status = (int)LightStatus::Enabled;
		}
		else
		{
			light->status = (int)LightStatus::Disabled;
		}
	}

}
