#pragma once
#include "../HODO3DGraphicsInterface/IParticle.h"
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;

	class Particle : public HDEngine::IParticle
	{
	public:
		Particle();
		~Particle();

		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void SetMesh(const std::string& meshName) override;
		virtual void SetMaterial(HDEngine::IMaterial* material) override;
		virtual void SetColor(DirectX::XMINT4 color) override;
		virtual void SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode) override;

		void Render();

	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> _rasterizerState;
		DirectX::XMMATRIX _world;
		Mesh* _mesh;
		Material* _material;
		bool _isActive;
		HDEngine::ParticleSystemRenderMode _renderMode;
	};
}
