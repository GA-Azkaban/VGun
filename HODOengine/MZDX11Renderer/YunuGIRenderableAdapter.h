#pragma once
#include <unordered_map>
#include "YunuGraphicsInterface.h"
#include "IYunuGIRenderable.h"
#include "IRenderableObject.h"

namespace yunuGIAdapter
{
	class RenderableAdapter : virtual public yunuGI::IRenderable
	{
	public:
		RenderableAdapter(IRenderableObject* obj)
			: m_pRenderableObject(obj)
		{
			wrapperMap[obj] = this;
		}
		static RenderableAdapter* GetWrapper(IRenderableObject* instance)
		{
			if (auto itr = wrapperMap.find(instance); itr == wrapperMap.end())
				return nullptr;
			else
				return itr->second;
		}
	private:
		static std::unordered_map<IRenderableObject*, RenderableAdapter*> wrapperMap;
		std::unique_ptr<IRenderableObject> m_pRenderableObject;

	};
}
