#pragma once
#include "../HODO3DGraphicsInterface/IRenderable.h"
#include <concepts>
#include <unordered_map>
#include "Component.h"
#include "Transform.h"

/// <summary>
/// ȭ�鿡 �׷��� �� �ִ� ��ü���� �θ� �Ǵ� Ŭ����
/// </summary>

namespace hodoEngine
{
	template <class RenderObjectType> requires std::derived_from<RenderObjectType, hodoGI::IRenderable>
	class Renderable : public Component
	{
	public:
		virtual ~Renderable()
		{
			_renderObjectMap.erase(_renderableObject);
		}

		virtual void Update() override
		{
			_renderableObject->SetWorldTM(GetTransform()->GetWorldTM());
		}

	protected:
		Renderable(RenderObjectType* object)
			:_instance(object)
		{
			_renderableObject = static_cast<hodoGI::IRenderable*>(_instance);
			_renderObjectMap[_renderableObject] = this;
		}

		virtual void OnEnable()
		{
			_renderableObject->SetActive(true);
		}

		virtual void OnDisable()
		{
			_renderableObject->SetActive(false);
		}

		RenderObjectType* Get()
		{
			return _instance;
		}

	private:
		static std::unordered_map<hodoGI::IRenderable*, Renderable<RenderObjectType>*> _renderObjectMap;
		hodoGI::IRenderable* _renderableObject;
		RenderObjectType* _instance;
	};
}

template <class RenderObjectType> requires std::derived_from<RenderObjectType, hodoGI::IRenderable>
std::unordered_map<hodoGI::IRenderable*, hodoEngine::Renderable<RenderObjectType>*> hodoEngine::Renderable<RenderObjectType>::_renderObjectMap;