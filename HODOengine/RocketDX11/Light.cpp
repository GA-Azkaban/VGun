#include "Light.h"
#include "Camera.h"
#include <exception>

namespace RocketCore::Graphics
{

	LightManager::LightManager()
		: _numLights(0)
	{

	}

	LightManager::~LightManager()
	{

	}

	Light* LightManager::AddLight()
	{
		if (_numLights < MAX_LIGHTS)
		{
			Light light;
			_lightProperties.lights[_numLights] = light;
			return &_lightProperties.lights[_numLights++];
		}
		else
		{
			throw std::exception("LightManager:: Too many lights added");
			return nullptr;
		}
	}

	Light& LightManager::GetLight(unsigned int index)
	{
		if (index < MAX_LIGHTS)
		{
			return _lightProperties.lights[index];
		}
		else
		{
			throw std::exception("LightManager:: light index out of range");
		}
	}

	LightProperties& LightManager::GetLightProperties()
	{
		return _lightProperties;
	}

	void LightManager::SetCameraPosition(DirectX::XMFLOAT4 cameraPosition)
	{
		_lightProperties.cameraPosition = cameraPosition;
	}

	void LightManager::SetGlobalAmbient(DirectX::XMFLOAT4 color)
	{
		_lightProperties.globalAmbient = color;
	}

}
