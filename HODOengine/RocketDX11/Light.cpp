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

		static float const far_factor = 1.5f;
		static float const light_distance_factor = 1.0f;

		Camera* mainCamera = Camera::GetMainCamera();
		XMMATRIX invView = XMMatrixInverse(nullptr, mainCamera->GetViewMatrix());

		for(UINT i = 0; i < ShadowMapPass::CASCADE_COUNT; ++i)
		BoundingFrustum frustum(mainCamera->GetProjectionMatrix(i));
		frustum.Transform(frustum, invView);
		std::array<Vector3, BoundingFrustum::CORNER_COUNT> corners{};
		frustum.GetCorners(corners.data());
		
		Vector3 frustum_center(0, 0, 0);
		for (Vector3 const& corner : corners)
		{
			frustum_center = frustum_center + corner;
		}
		frustum_center /= static_cast<float>(corners.size());

		float radius = 0.0f;
		for (Vector3 const& corner : corners)
		{
			float distance = Vector3::Distance(corner, frustum_center);
			radius = std::max(radius, distance);
		}
		radius = std::ceil(radius * 8.0f) / 8.0f;

		// make aabb box using radius
		Vector3 const max_extents(radius, radius, radius);
		Vector3 const min_extents = -max_extents;

		Vector3 lightDirNorm = XMVector3Normalize(XMLoadFloat4(&(dirLight->direction)));
		Matrix V = XMMatrixLookAtLH(frustum_center, frustum_center + light_distance_factor * lightDirNorm * radius, Vector3::Up);

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
		shadow_origin *= (ShadowMapPass::SHADOW_MAP_SIZE / 2.0f);

		Vector3 rounded_origin = XMVectorRound(shadow_origin);
		Vector3 rounded_offset = rounded_origin - shadow_origin;
		rounded_offset *= (2.0f / ShadowMapPass::SHADOW_MAP_SIZE);
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
