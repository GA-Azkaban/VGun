#pragma once
#include "../HODO3DGraphicsInterface/ILight.h"
#include "Light.h"

namespace RocketCore::Graphics
{
	class LightAdapter : public HDEngine::ILight
	{
	public:
		LightAdapter();

		virtual void SetPosition(const Vector4& position) override;
		virtual void SetDirection(const Vector4& direction) override;
		virtual void SetColor(const Vector4& color) override;
		virtual void SetLightType(unsigned int type) override;
		virtual void SetActive(bool isActive) override;

	private:
		Light* light;
	};
}
