#pragma once
#include "../HODO3DGraphicsInterface/IParticle.h"
#include <d3d11.h>
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class Particle : public HDEngine::IParticle
	{
	public:
		Particle();
		~Particle();

		virtual DirectX::XMMATRIX GetWorldTM() const override;
		virtual DirectX::XMINT4 GetColor() const override;
		virtual DirectX::XMFLOAT4 GetColorFloat4() const override;
		virtual float GetSpeed() const override;
		virtual float GetSize() const override;
		virtual float GetStartSize() const override;
		virtual float GetAngle() const override;
		virtual DirectX::XMFLOAT3 GetPosition() const override;
		virtual DirectX::XMFLOAT4 GetRotation() const override;
		virtual DirectX::XMFLOAT3 GetScale() const override;

		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void SetColor(DirectX::XMINT4 color) override;
		virtual void SetSpeed(float speed) override;
		virtual void SetSize(float size) override;
		virtual void SetStartSize(float size) override;
		virtual void SetAngle(float angle) override;
		virtual void SetPosition(float x, float y, float z) override;
		virtual void SetPosition(DirectX::XMFLOAT3 pos) override;
		virtual void SetRotation(float x, float y, float z, float w) override;
		virtual void SetRotation(DirectX::XMFLOAT4 rot) override;
		virtual void SetScale(float x, float y, float z) override;
		virtual void SetScale(DirectX::XMFLOAT3 scale) override;

	private:
		DirectX::XMMATRIX _world;
		bool _isActive;

		DirectX::XMINT4 _color;
		float _speed;
		float _size;
		float _startSize;
		float _angle;

		DirectX::XMFLOAT3 _position;
		DirectX::XMFLOAT4 _rotation;
		DirectX::XMFLOAT3 _scale;
	};
}
