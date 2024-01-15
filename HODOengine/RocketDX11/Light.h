#pragma once
#include <DXTK/SimpleMath.h>

namespace RocketCore::Graphics
{
	class Light
	{
	public:
		void SetDiffuseColor(float r, float g, float b, float a);
		void SetDirection(float x, float y, float z);

		DirectX::XMFLOAT4 GetDiffuseColor();
		DirectX::XMFLOAT3 GetDirection();

	private:
		DirectX::XMFLOAT4 _diffuseColor;
		DirectX::XMFLOAT3 _direction;
	};
}
