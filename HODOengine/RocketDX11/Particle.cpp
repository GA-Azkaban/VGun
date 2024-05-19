#include "Particle.h"

using namespace DirectX;

namespace RocketCore::Graphics
{

	Particle::Particle()
		: _world(DirectX::XMMatrixIdentity()), _isActive(true),
		_color{ 255, 255, 255, 255 }, _speed(5.0f),
		_size(1.0f), _angle(0.0f),
		_position{0.0f, 0.0f, 0.0f}, _rotation{0.0f, 0.0f, 0.0f, 1.0f}, _scale{1.0f, 1.0f, 1.0f}
	{
		
	}

	Particle::~Particle()
	{
		
	}

	DirectX::XMMATRIX Particle::GetWorldTM() const
	{
		return _world;
	}

	DirectX::XMINT4 Particle::GetColor() const
	{
		return _color;
	}

	DirectX::XMFLOAT4 Particle::GetColorFloat4() const
	{
		return DirectX::XMFLOAT4{ _color.x / 255.0f, _color.y / 255.0f, _color.z / 255.0f, _color.w / 255.0f };
	}

	float Particle::GetSpeed() const
	{
		return _speed;
	}

	float Particle::GetSize() const
	{
		return _size;
	}

	float Particle::GetAngle() const
	{
		return _angle;
	}

	DirectX::XMFLOAT3 Particle::GetPosition() const
	{
		return _position;
	}

	DirectX::XMFLOAT4 Particle::GetRotation() const
	{
		return _rotation;
	}

	DirectX::XMFLOAT3 Particle::GetScale() const
	{
		return _scale;
	}

	void Particle::SetWorldTM(const Matrix& worldTM)
	{
		_world = worldTM;
	}

	void Particle::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void Particle::SetColor(DirectX::XMINT4 color)
	{
		_color = color;
	}

	void Particle::SetSpeed(float speed)
	{
		_speed = speed;
	}

	void Particle::SetSize(float size)
	{
		_size = size;
	}

	void Particle::SetAngle(float angle)
	{
		_angle = angle;
	}

	void Particle::SetPosition(float x, float y, float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	void Particle::SetPosition(DirectX::XMFLOAT3 pos)
	{
		_position = pos;
	}

	void Particle::SetRotation(float x, float y, float z, float w)
	{
		_rotation.x = x;
		_rotation.y = y;
		_rotation.z = z;
		_rotation.w = w;
	}

	void Particle::SetRotation(DirectX::XMFLOAT4 rot)
	{
		_rotation = rot;
	}

	void Particle::SetScale(float x, float y, float z)
	{
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;
	}

	void Particle::SetScale(DirectX::XMFLOAT3 scale)
	{
		_scale = scale;
	}

}
