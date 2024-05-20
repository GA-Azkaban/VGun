#include "ParticleSystem.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "ParticlePool.h"
#include "VertexShader.h"
#include "PixelShader.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	std::array<DirectX::XMMATRIX, ParticleSystem::maxParticleCount> ParticleSystem::particleTransforms;
	std::array<DirectX::XMFLOAT4, ParticleSystem::maxParticleCount> ParticleSystem::particleColors;

	ParticleSystem::ParticleSystem()
		: _mesh(nullptr), _material(nullptr),
		_renderMode(HDEngine::ParticleSystemRenderMode::Billboard), _isActive(true)
	{
		_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
		_mesh = ResourceManager::Instance().GetMeshes("primitiveQuad")[0];
		_material = ResourceManager::Instance().GetLoadedMaterial("Default-ParticleSystem");
		_material->SetVertexShader(ResourceManager::Instance().GetVertexShader("ParticleVertexShader.cso"));
		_material->SetPixelShader(ResourceManager::Instance().GetPixelShader("ParticlePixelShader.cso"));
	}

	ParticleSystem::~ParticleSystem()
	{
		_mesh = nullptr;
		_material = nullptr;
	}

	void ParticleSystem::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void ParticleSystem::SetMesh(const std::string& meshName)
	{
		if (meshName != "")
		{
			_mesh = ResourceManager::Instance().GetMeshes(meshName)[0];
		}
	}

	void ParticleSystem::SetMaterial(HDEngine::IMaterial* material)
	{
		Material* newMat = dynamic_cast<Material*>(material);
		if (newMat != nullptr)
		{
			_material = newMat;
			_material->SetVertexShader(ResourceManager::Instance().GetVertexShader("ParticleVertexShader.cso"));
			_material->SetPixelShader(ResourceManager::Instance().GetPixelShader("ParticlePixelShader.cso"));
		}
	}

	void ParticleSystem::SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode)
	{
		_renderMode = renderMode;

		if (renderMode == HDEngine::ParticleSystemRenderMode::Billboard)
		{
			_mesh = ResourceManager::Instance().GetMeshes("primitiveQuad")[0];
		}
		//else if (_renderMode == HDEngine::ParticleSystemRenderMode::Mesh)
		//{
		//	//_material->SetVertexShader()
		//}
	}

	HDEngine::IParticle* ParticleSystem::SummonParticle()
	{
		if (_activatedParticles.size() <= maxParticleCount)
		{
			HDEngine::IParticle* newParticle = ParticlePool::Instance().SummonParticle();
			_activatedParticles.insert(newParticle);
			return newParticle;
		}
		return nullptr;
	}

	void ParticleSystem::DestroyParticle(HDEngine::IParticle* particle)
	{
		if (particle != nullptr)
		{
			auto iter = _activatedParticles.find(particle);
			if (iter != _activatedParticles.end())
			{
				ParticlePool::Instance().Retrieve(particle);
				_activatedParticles.erase(iter);
			}
		}
	}

	void ParticleSystem::Render()
	{
		if (!_isActive)
		{
			return;
		}

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(_rasterizerState.Get());

		VertexShader* vs = _material->GetVertexShader();
		PixelShader* ps = _material->GetPixelShader();

		int index = 0;
		for (auto e : _activatedParticles)
		{
			particleTransforms[index] = XMMatrixTranspose(e->GetWorldTM());
			particleColors[index] = e->GetColorFloat4();
			++index;
		}
		
		vs->SetMatrix4x4Array("particleTransforms", &particleTransforms[0], maxParticleCount);
		vs->SetFloat4Array("particleColors", &particleColors[0], maxParticleCount);
		vs->CopyAllBufferData();
		vs->SetShader();

		ps->SetShaderResourceView("Albedo", _material->GetAlbedoMap());
		ps->SetFloat4("albedoColor", _material->GetColorFloat4()); 

		ps->CopyAllBufferData();
		ps->SetShader();

		_mesh->BindBuffers();
		_mesh->DrawInstanced(_activatedParticles.size());
	}

}
