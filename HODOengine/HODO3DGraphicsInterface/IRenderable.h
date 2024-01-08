#pragma once

namespace HDMath
{
	struct HDFLOAT4X4;
}

namespace HDEngine
{
	class IRenderable
	{
	public:
		virtual ~IRenderable() {}
		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& worldTM) = 0;
		virtual void SetActive(bool isActive) = 0;
	};
}