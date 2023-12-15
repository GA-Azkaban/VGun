#pragma once

namespace HDMath
{
	struct HDFLOAT4X4;
}

namespace HDEngine
{
	class ICamera
	{
	public:
		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& matrix) = 0;
		virtual void SetNearZ(float near) = 0;
		virtual void SetFarZ(float far) = 0;
		virtual void SetAspect(float aspect) = 0;
		virtual void SetFOVY(float fov) = 0;
		virtual void SetNearHeight(float height) = 0;
		virtual void SetFarHeight(float height) = 0;
	};
}