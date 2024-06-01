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

		static float const far_factor = 1.5f;
		static float const light_distance_factor = 1.0f;
		static float const radius = 30.0f;

		Vector3 frustum_center(0, 0, 0);
		
		Vector3 cameraPos = Camera::GetMainCamera()->GetPosition();
		Vector3 cameraForward = Camera::GetMainCamera()->GetForward();
		
		frustum_center.x += (cameraPos.x);
		frustum_center.z += (cameraPos.z);

		frustum_center.x += cameraForward.x * radius;
		frustum_center.z += cameraForward.z * radius;

		Vector3 const max_extents(radius, radius, radius);
		Vector3 const min_extents = -max_extents;
		Vector3 const cascade_extents = max_extents - min_extents;

		Vector3 light_dir = XMVector3Normalize(XMLoadFloat4(&(dirLight->direction)));
		Matrix V = XMMatrixLookAtLH(frustum_center, frustum_center + light_distance_factor * light_dir * radius, Vector3::Up);

		float l = min_extents.x;
		float b = min_extents.y;
		float n = min_extents.z - far_factor * radius;
		float r = max_extents.x;
		float t = max_extents.y;
		float f = max_extents.z * far_factor;

		Matrix P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
		
		Matrix VP = V * P;
		Vector3 shadow_origin(0, 0, 0);
		shadow_origin = Vector3::Transform(shadow_origin, VP);
		shadow_origin *= (500.0f / 2.0f);

		Vector3 rounded_origin = XMVectorRound(shadow_origin);
		Vector3 rounded_offset = rounded_origin - shadow_origin;
		rounded_offset *= (2.0f / 500.0f);
		rounded_offset.z = 0.0f;
		P.m[3][0] += rounded_offset.x;
		P.m[3][1] += rounded_offset.y;

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
