#include "ParticleSystemRenderer.h"

namespace HDData
{

	ParticleSystemRenderer::ParticleSystemRenderer()
		: _renderMode(HDEngine::ParticleSystemRenderMode::Billboard),
		_material(nullptr), _mesh(""), _minParticleSize(0), _maxParticleSize(0)
	{

	}

	ParticleSystemRenderer::~ParticleSystemRenderer()
	{
		_material = nullptr;
	}

	HDEngine::ParticleSystemRenderMode ParticleSystemRenderer::GetRenderMode() const
	{
		return _renderMode;
	}

	HDData::Material* ParticleSystemRenderer::GetMaterial() const
	{
		return _material;
	}

	std::string ParticleSystemRenderer::GetMesh() const
	{
		return _mesh;
	}

	float ParticleSystemRenderer::GetMinParticleSize() const
	{
		return _minParticleSize;
	}

	float ParticleSystemRenderer::GetMaxParticleSize() const
	{
		return _maxParticleSize;
	}

	void ParticleSystemRenderer::SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode)
	{
		_renderMode = renderMode;
	}

	void ParticleSystemRenderer::SetMaterial(HDData::Material* material)
	{
		_material = material;
	}

	void ParticleSystemRenderer::SetMesh(std::string meshName)
	{
		_mesh = meshName;
	}

	void ParticleSystemRenderer::SetParticleSize(float minSize, float maxSize)
	{
		_minParticleSize = minSize;
		_maxParticleSize = maxSize;
	}

	void ParticleSystemRenderer::Update()
	{

	}

}
