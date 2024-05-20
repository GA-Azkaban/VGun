#pragma once
#include "IRenderable.h"

namespace HDEngine
{
	class IParticle : public IRenderable
	{
	public:
		virtual ~IParticle() {}
		virtual void SetColor(DirectX::XMINT4 color) = 0;
		virtual void SetSpeed(float speed) = 0;
		virtual void SetSize(float size) = 0;
		virtual void SetStartSize(float size) = 0;
		virtual void SetAngle(float angle) = 0;
		virtual void SetPosition(float x, float y, float z) = 0;
		virtual void SetPosition(DirectX::XMFLOAT3 pos) = 0;
		virtual void SetRotation(float x, float y, float z, float w) = 0;
		virtual void SetRotation(DirectX::XMFLOAT4 rot) = 0;
		virtual void SetScale(float x, float y, float z) = 0;
		virtual void SetScale(DirectX::XMFLOAT3 scale) = 0;

		virtual DirectX::XMMATRIX GetWorldTM() const = 0;
		virtual DirectX::XMINT4 GetColor() const = 0;
		virtual DirectX::XMFLOAT4 GetColorFloat4() const = 0;
		virtual float GetSpeed() const = 0;
		virtual float GetSize() const = 0;
		virtual float GetStartSize() const = 0;
		virtual float GetAngle() const = 0;
		virtual DirectX::XMFLOAT3 GetPosition() const = 0;
		virtual DirectX::XMFLOAT4 GetRotation() const = 0;
		virtual DirectX::XMFLOAT3 GetScale() const = 0;
	};
}
