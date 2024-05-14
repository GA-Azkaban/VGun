#include "Particle.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
using namespace DirectX;

namespace RocketCore::Graphics
{

	Particle::Particle()
		: _world(DirectX::XMMatrixIdentity()), _mesh(nullptr), _material(nullptr),
		_isActive(true), _renderMode(HDEngine::ParticleSystemRenderMode::Billboard),
		_speed(0.0f), _size(1.0f), _angle(0.0f)
	{
		_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);

		_mesh = ResourceManager::Instance().GetMeshes("quadMesh")[0];
		_material = ResourceManager::Instance().GetLoadedMaterial("Default");
		_material->SetVertexShader(ResourceManager::Instance().GetVertexShader("BillboardVertexShader.cso"));
		_material->SetPixelShader(ResourceManager::Instance().GetPixelShader("BillboardPixelShader.cso"));
	}

	Particle::~Particle()
	{
		_mesh = nullptr;
		_material = nullptr;
	}

	void Particle::SetWorldTM(const Matrix& worldTM)
	{
		_world = worldTM;
	}

	void Particle::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void Particle::SetMesh(const std::string& meshName)
	{
		_mesh = ResourceManager::Instance().GetMeshes(meshName)[0];
	}

	void Particle::SetMaterial(HDEngine::IMaterial* material)
	{
		Material* newMat = dynamic_cast<Material*>(material);
		if (newMat != nullptr)
		{
			_material = newMat;
			if (_renderMode == HDEngine::ParticleSystemRenderMode::Mesh)
			{
				_material->SetVertexShader()
			}
		}
	}

	void Particle::SetColor(DirectX::XMINT4 color)
	{
		if (_material != nullptr)
		{
			_material->SetColor(color.x, color.y, color.z, color.w);
		}
	}

	void Particle::SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode)
	{
		_renderMode = renderMode;

		if (renderMode == HDEngine::ParticleSystemRenderMode::Billboard)
		{
			_mesh = ResourceManager::Instance().GetMeshes("quadMesh")[0];
		}
	}

	void Particle::SetSpeed(float speed)
	{
		_speed = speed;
	}

	void Particle::SetSize(float size)
	{
		_size = size;
	}

	void Particle::SetRotation(float angle)
	{
		_angle = angle;
	}

	void Particle::Update(float deltaTime)
	{

	}

	void Particle::Render()
	{
		if (!_isActive)
		{
			return;
		}
	}

}
