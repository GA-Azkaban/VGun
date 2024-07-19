#pragma once
#include "../HODO3DGraphicsInterface/IParticleSystem.h"
#include <d3d11.h>
#include <wrl.h>
#include <unordered_set>
#include <array>
#include <DirectXMath.h>

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
		virtual void SetAlphaBlend(bool isAlphaBlend) override;
		void Render();
		bool IsAlphaBlend() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> _rasterizerState;
		Mesh* _mesh;
		Material* _material;
		HDEngine::ParticleSystemRenderMode _renderMode;
		bool _isActive;
		bool _isAlphaBlend;

		std::unordered_set<HDEngine::IParticle*> _activatedParticles;

		static constexpr unsigned int maxParticleCount = 512;
		static std::array<DirectX::XMMATRIX, maxParticleCount> particleTransforms;
		static std::array<DirectX::XMFLOAT4, maxParticleCount> particleColors;
	};
}
