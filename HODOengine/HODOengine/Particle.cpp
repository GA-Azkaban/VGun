#include "Particle.h"
#include "GraphicsObjFactory.h"

namespace HDData
{

	Particle::Particle()
		: _particle(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateParticle()),
		_renderMode(HDEngine::ParticleSystemRenderMode::Billboard),
		_mesh(""), _material(nullptr), _color{255, 255, 255, 255},
		_speed(5.0f), _size(1.0f), _angle(0.0f),
		_position{0.0f, 0.0f, 0.0f}, _rotation{0.0f, 0.0f, 0.0f, 1.0f}, _scale{1.0f, 1.0f, 1.0f}
	{

	}

	HDEngine::IParticle& Particle::Get()
	{
		return *_particle;
	}

	HDEngine::ParticleSystemRenderMode Particle::GetRenderMode() const
	{
		return _renderMode;
	}

	std::string Particle::GetMesh() const
	{
		return _mesh;
	}

	HDData::Material* Particle::GetMaterial() const
	{
		return _material;
	}

	DirectX::XMINT4 Particle::GetColor() const
	{
		return _color;
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

	Vector3 Particle::GetPosition() const
	{
		return _position;
	}

	Quaternion Particle::GetRotation() const
	{
		return _rotation;
	}

	Vector3 Particle::GetScale() const
	{
		return _scale;
	}

	void Particle::SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode)
	{
		_renderMode = renderMode;
	}

	void Particle::SetMesh(const std::string& name)
	{
		_mesh = name;
	}

	void Particle::SetMaterial(HDData::Material* material)
	{
		_material = material;
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

	void Particle::SetPosition(Vector3 v)
	{
		_position = v;
	}

	void Particle::SetRotation(float x, float y, float z, float w)
	{
		_rotation.x = x;
		_rotation.y = y;
		_rotation.z = z;
		_rotation.w = w;
	}

	void Particle::SetRotation(Quaternion q)
	{
		_rotation = q;
	}

	void Particle::SetScale(float x, float y, float z)
	{
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;
	}

}
