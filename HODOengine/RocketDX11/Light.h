#pragma once
#include "Singleton.h"
#include <DirectXMath.h>
#define MAX_LIGHTS 1

namespace RocketCore::Graphics
{
	enum class LightType
	{
		DirectionalLight,
		PointLight,
		SpotLight,
	};

	enum class LightStatus
	{
		Disabled,
		Enabled,
		Static_Shadows,
	};

	//_declspec(align(16)) struct Light
	struct Light
	{
		//DirectX::XMFLOAT4 position{0.0f, 0.0f, 0.0f, 0.0f};
		DirectX::XMFLOAT4 direction{-1.0f, -1.0f, 1.0f, 0.0f};
		DirectX::XMFLOAT4 color{5.0f, 5.0f, 5.0f, 1.0f};

		//float range = 2.0f;
		//float spotAngle = 0.0f;

		int lightType = (int)LightType::DirectionalLight;
		int status = (int)LightStatus::Enabled;
	};

	//_declspec(align(16)) struct LightProperties
	struct LightProperties
	{
		DirectX::XMFLOAT4 cameraPosition;
		DirectX::XMFLOAT4 globalAmbient;
		Light lights[MAX_LIGHTS];
	};

	class LightManager : public Singleton<LightManager>
	{
	public:
		friend Singleton;
		~LightManager();

		Light* AddLight();
		Light& GetLight(unsigned int index);
		LightProperties& GetLightProperties();
		void SetCameraPosition(DirectX::XMFLOAT4 cameraPosition);
		void SetGlobalAmbient(DirectX::XMFLOAT4 color);

	private:
		LightManager();
		LightProperties _lightProperties;
		unsigned int _numLights;
	};
}
