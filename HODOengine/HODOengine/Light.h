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

	private:
		HDEngine::ILight* _light;
	};
}
