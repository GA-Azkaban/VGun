#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "MathHeader.h"
#include "../HODO3DGraphicsInterface/ILight.h"

namespace HDData
{
	class HODO_API Light : public Component
	{
	public:
		enum LightType
		{
			DirectionalLight,
		};

		Light();

		void SetPosition(const Vector4& position);
		void SetDirection(const Vector4& direction);
		void SetColor(const Vector4& color);
		void SetLightType(LightType type);		
		void SetActive(bool isActive);

		virtual void Update() override;

	private:
		HDEngine::ILight* _light;

		// 이거 Light Component를 가진 오브젝트의 Transform을 통해 계산할 수 있다
		// 일단 이렇게 받는다
		Vector4 _lightPosition;
		Vector4 _lightDirection;
		Vector4 _lightColor;
		LightType _lightType;
		bool _isActive;
	};
}
