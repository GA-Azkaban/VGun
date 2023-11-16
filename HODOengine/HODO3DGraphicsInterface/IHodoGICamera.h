#pragma once

namespace HDMaths
{
	struct HDFLOAT4X4;
}

namespace hodoGI
{
	class ICamera
	{
	public:
		virtual void SetWorldTM(const HDMaths::HDFLOAT4X4&) = 0;
		virtual void SetFOV(float fov) = 0;
		virtual void SetNear(float near) = 0;
		virtual void SetFar(float far) = 0;
		virtual void SetAsMainCamera() = 0;
	};
}