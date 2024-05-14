#pragma once
#include "../HODO3DGraphicsInterface/IParticle.h"
#include "MathHeader.h"

namespace HDData
{
	class Material;

	class Particle
	{
	public:
		Particle();
		HDEngine::IParticle& Get();

		HDEngine::ParticleSystemRenderMode GetRenderMode() const;
		std::string GetMesh() const;
		HDData::Material* GetMaterial() const;
		DirectX::XMINT4 GetColor() const;
		float GetSpeed() const;
		float GetSize() const;
		float GetAngle() const;
		Vector3 GetPosition() const;
		Quaternion GetRotation() const;
		Vector3 GetScale() const;
	
		void SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode);
		void SetMesh(const std::string& name);
		void SetMaterial(HDData::Material* material);
		void SetColor(DirectX::XMINT4 color);
		void SetSpeed(float speed);
		void SetSize(float size);
		void SetAngle(float angle);
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z, float w);
		void SetRotation(Quaternion q);
		void SetScale(float x, float y, float z);

	private:
		HDEngine::IParticle* _particle;
		HDEngine::ParticleSystemRenderMode _renderMode;
		std::string _mesh;
		HDData::Material* _material;
		DirectX::XMINT4 _color;
		float _speed;
		float _size;
		float _angle;

		Vector3 _position;
		Quaternion _rotation;
		Vector3 _scale;
	};
}
