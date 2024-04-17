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

		static float const far_factor = 3.5f;
		static float const light_distance_factor = 1.0f;
		static float const radius = 38.0f;

		Vector3 frustum_center(0, 0, 0);
		
		Vector3 cameraPos = Camera::GetMainCamera()->GetPosition();
		Vector3 cameraForward = Camera::GetMainCamera()->GetForward();
		
		frustum_center.x += std::ceil(cameraPos.x);
		frustum_center.y += std::ceil(cameraPos.y);
		frustum_center.z += std::ceil(cameraPos.z);

		frustum_center.x += cameraForward.x * radius;
		frustum_center.y += cameraForward.y * radius;
		frustum_center.z += cameraForward.z * radius;

		Vector3 const max_extents(radius, radius, radius);
		Vector3 const min_extents = -max_extents;

		Vector3 light_dir = XMVector3Normalize(XMLoadFloat4(&(dirLight->direction)));
		Matrix V = XMMatrixLookAtLH(frustum_center, frustum_center + light_distance_factor * light_dir * radius, Vector3::Up);

		float l = min_extents.x;
		float b = min_extents.y;
		float n = min_extents.z - far_factor * radius;
		float r = max_extents.x;
		float t = max_extents.y;
		float f = max_extents.z * far_factor;

		Matrix P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
		
		_lightView = V;
		_lightProj = P;
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
