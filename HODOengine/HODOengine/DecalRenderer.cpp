#include "DecalRenderer.h"
#include "GraphicsObjFactory.h"
#include "HDMaterial.h"
#include "Transform.h"

namespace HDData
{

	DecalRenderer::DecalRenderer()
		: _decal(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateDecalObject())
	{

	}

	DecalRenderer::~DecalRenderer()
	{
		_decal = nullptr;
		delete _decal;
	}

	void DecalRenderer::LoadMaterial(HDData::Material* material)
	{
		_decal->LoadMaterial(material->Get());
	}

	void DecalRenderer::SetColor(UINT r, UINT g, UINT b, UINT a)
	{
		_decal->SetColor(r, g, b, a);
	}

	void DecalRenderer::OnEnable()
	{
		_decal->SetActive(true);
	}

	void DecalRenderer::OnDisable()
	{
		_decal->SetActive(false);
	}

	void DecalRenderer::OnDestroy()
	{
		HDEngine::GraphicsObjFactory::Instance().GetFactory()->DestroyDecalObject(_decal);
	}

	void DecalRenderer::Update()
	{
		_decal->SetWorldTM(GetTransform()->GetWorldTM());
	}

}
