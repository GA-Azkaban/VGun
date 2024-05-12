#pragma once
#include "../HODO3DGraphicsInterface/IParticle.h"
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;

	class Particle : public HDEngine::IParticle
	{
	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void SetMesh(const std::string& meshName) override;
		virtual void SetMaterial(HDEngine::IMaterial* material) override;
		virtual void SetColor(DirectX::XMINT4 color) override;
		virtual void SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode) override;

		void Update(float deltaTime);
		void Render();

	private:
		DirectX::XMMATRIX _world;
		Mesh* _mesh;
		Material* _material;
		bool _isActive;
		HDEngine::ParticleSystemRenderMode _renderMode;
	};
}
