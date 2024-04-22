#include "Light.h"
#include "Camera.h"
#include "ShadowMapPass.h"
#include <exception>
#include <array>
using namespace DirectX;

namespace RocketCore::Graphics
{

	LightManager::LightManager()
		: _numLights(0)
	{
		//_lightProj = XMMatrixOrthographicLH(300, 300, 1, 600);
	}

	LightManager::~LightManager()
	{

	}

	void LightManager::UpdateViewProj()
	{
		Light* dirLight = nullptr;
		for (UINT i = 0; i < _numLights; ++i)
		{
			if (_lights[i].lightType == (int)LightType::DirectionalLight)
			{
				dirLight = &(_lights[i]);
			}
		}
		Camera* mainCamera = Camera::GetMainCamera();
		XMMATRIX invView = XMMatrixInverse(nullptr, mainCamera->GetViewMatrix());

		float fovZ = mainCamera->GetFOVZ();
		
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
		return _lightView;
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
