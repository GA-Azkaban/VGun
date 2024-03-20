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

	class LightManager : public Singleton<LightManager>
	{
	public:
		friend Singleton;
		~LightManager();

		Light* AddLight();
		Light& GetLight(unsigned int index);
		
		DirectX::XMFLOAT4 GetGlobalAmbient();
		void SetGlobalAmbient(DirectX::XMFLOAT4 color);

		DirectX::XMMATRIX GetLightView();
		DirectX::XMMATRIX GetLightProj();
		DirectX::XMMATRIX GetLightViewProj();

	private:
		LightManager();

		DirectX::XMFLOAT4 _globalAmbient;

		Light _lights[MAX_LIGHTS];
		unsigned int _numLights;

		DirectX::XMMATRIX _lightView;
		DirectX::XMMATRIX _lightProj;
	};
}
