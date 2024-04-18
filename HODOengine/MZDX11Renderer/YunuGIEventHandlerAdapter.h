#pragma once
#include "IYunuGIEventHandler.h"
#include "YunuGIRenderableAdapter.h"
#include "MZDX11Renderer.h"

class IRenderableObject;

namespace yunuGIAdapter
{
	class EventHandlerAdapter : public yunuGI::IEventHandler
	{
	public:
		virtual void AddClickCallback(CallbackPtr callback) override
		{
			clickCallback = callback;
		}
		virtual yunuGI::IRenderable* Click(float normalizedX, float normalizedY) override
		{
			IRenderableObject* obj = MZDX11Renderer::Instance.Get().Pick(normalizedX, normalizedY);
			return static_cast<yunuGI::IRenderable*>(RenderableAdapter::GetWrapper(obj));
		}

	private:
		CallbackPtr clickCallback{ nullptr };
	};
}