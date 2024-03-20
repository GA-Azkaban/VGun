#include "Light.h"
#include "Camera.h"
#include <exception>
using namespace DirectX;

namespace RocketCore::Graphics
{

	LightManager::LightManager()
		: _numLights(0)
	{
		_lightProj = XMMatrixOrthographicLH(16, 12, 1, 50);
	}

	LightManager::~LightManager()
	{

	}

	Light* LightManager::AddLight()
	{
		if (_numLights < MAX_LIGHTS)
		{
			Light light;
			_lights[_numLights] = light;
			return &_lights[_numLights++];
		}
		else
		{
			return &_lights[MAX_LIGHTS - 1];
		}
	}

	Light& LightManager::GetLight(unsigned int index)
	{
		if (index < MAX_LIGHTS)
		{
			return _lights[index];
		}
		else
		{
			throw std::exception("LightManager:: light index out of range");
		}
	}

	DirectX::XMFLOAT4 LightManager::GetGlobalAmbient()
	{
		return _globalAmbient;
	}

	void LightManager::SetGlobalAmbient(DirectX::XMFLOAT4 color)
	{
		_globalAmbient = color;
	}

	DirectX::XMMATRIX LightManager::GetLightView()
	{
		Light* dirLight = nullptr;
		for (UINT i = 0; i < _numLights; ++i)
		{
			if (_lights[i].lightType == (int)LightType::DirectionalLight)
			{
				dirLight = &(_lights[i]);
			}
		}

		if (dirLight != nullptr)
		{
			XMVECTOR lightDirection = XMLoadFloat4(&dirLight->direction);
			// 일단 -10을 곱한다. 나중에 고쳐야 한다.
			_lightView = XMMatrixLookAtLH(-10 * lightDirection, XMVECTOR{ 0, 0, 0, 1 }, XMVECTOR{ 0, 1, 0, 0 });
			return _lightView;
		}

		return XMMatrixIdentity();
	}

	DirectX::XMMATRIX LightManager::GetLightProj()
	{
		return _lightProj;
	}

	DirectX::XMMATRIX LightManager::GetLightViewProj()
	{
		return GetLightView() * GetLightProj();
	}

}
