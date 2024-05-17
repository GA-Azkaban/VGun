#pragma once
#include "../HODO3DGraphicsInterface/IParticleSystem.h"
#include <wrl.h>
#include <unordered_set>

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;

	class ParticleSystem : public HDEngine::IParticleSystem
	{
	public:
		ParticleSystem();
		~ParticleSystem();
		
		virtual void SetActive(bool isActive) override;
		virtual void SetMesh(const std::string& meshName) override;
		virtual void SetMaterial(HDEngine::IMaterial* material) override;
		virtual void SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode) override;
		virtual HDEngine::IParticle* SummonParticle() override;
		virtual void DestroyParticle(HDEngine::IParticle* particle) override;

		void Render();

	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> _rasterizerState;
		Mesh* _mesh;
		Material* _material;
		HDEngine::ParticleSystemRenderMode _renderMode;
		bool _isActive;

		std::unordered_set<HDEngine::IParticle*> _activatedParticles;
	};
}
