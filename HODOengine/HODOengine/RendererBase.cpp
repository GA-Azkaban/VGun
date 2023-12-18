#include "RendererBase.h"
#include "Transform.h"

std::unordered_map<HDEngine::IRenderable*, HDData::RendererBase*> HDData::RendererBase::_renderableMap;

HDData::RendererBase::RendererBase(HDEngine::IRenderable* renderable)
	:_renderable(renderable)
{
	_renderableMap[renderable] = this;
}

void HDData::RendererBase::Update()
{
	_renderable->SetWorldTM(GetTransform()->GetWorldTM());
}

void HDData::RendererBase::OnEnable()
{
	_renderable->SetActive(true);
}

void HDData::RendererBase::OnDisable()
{
	_renderable->SetActive(false);
}
