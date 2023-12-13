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
		virtual void SetWorldTM(const HDMath::HDFLOAT4X4&) = 0;
		virtual void SetFOV(float fov) = 0;
		virtual void SetNear(float near) = 0;
		virtual void SetFar(float far) = 0;
		virtual void SetAsMainCamera() = 0;
	};
}