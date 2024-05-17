#include "ParticleSystem.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "ParticlePool.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace RocketCore::Graphics
{

	ParticleSystem::ParticleSystem()
		: _mesh(nullptr), _material(nullptr),
		_renderMode(HDEngine::ParticleSystemRenderMode::Billboard)
	{
		_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
		_mesh = ResourceManager::Instance().GetMeshes("quadMesh")[0];
		_material = ResourceManager::Instance().GetLoadedMaterial("Default-ParticleSystem");
		_material->SetVertexShader(ResourceManager::Instance().GetVertexShader("BillboardVertexShader.cso"));
		_material->SetPixelShader(ResourceManager::Instance().GetPixelShader("BillboardPixelShader.cso"));
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
		_mesh = ResourceManager::Instance().GetMeshes(meshName)[0];
	}

	void ParticleSystem::SetMaterial(HDEngine::IMaterial* material)
	{
		Material* newMat = dynamic_cast<Material*>(material);
		if (newMat != nullptr)
		{
			_material = newMat;
		}
	}

	void ParticleSystem::SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode)
	{
		_renderMode = renderMode;

		if (renderMode == HDEngine::ParticleSystemRenderMode::Billboard)
		{
			_mesh = ResourceManager::Instance().GetMeshes("quadMesh")[0];
			_material->SetVertexShader(ResourceManager::Instance().GetVertexShader("BillboardVertexShader.cso"));
			_material->SetPixelShader(ResourceManager::Instance().GetPixelShader("BillboardPixelShader.cso"));
		}
		//else if (_renderMode == HDEngine::ParticleSystemRenderMode::Mesh)
		//{
		//	//_material->SetVertexShader()
		//}
	}

	HDEngine::IParticle* ParticleSystem::SummonParticle()
	{
		HDEngine::IParticle* newParticle = ParticlePool::Instance().SummonParticle();
		_activatedParticles.insert(newParticle);
		return newParticle;
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

		for (auto e : _activatedParticles)
		{
			vs->SetMatrix4x4("world", XMMatrixTranspose(e->GetWorldTM()));
			//_vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));

			vs->CopyAllBufferData();
			vs->SetShader();

			ps->SetShaderResourceView("Albedo", _material->GetAlbedoMap());
			ps->SetFloat4("albedoColor", _material->GetColorFloat4());

			ps->CopyAllBufferData();
			ps->SetShader();

			_mesh->BindBuffers();
			_mesh->Draw();
		}
	}

}
