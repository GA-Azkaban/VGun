#pragma once

namespace HDMaths
{
	struct HDFLOAT4X4;
}

namespace hodoGI
{
	class IRenderable
	{
	public:
		virtual ~IRenderable() {};
		virtual void SetWorldTM(const HDMaths::HDFLOAT4X4& worldTM) = 0;
		virtual void SetActive(bool isActive) = 0;
	};
}