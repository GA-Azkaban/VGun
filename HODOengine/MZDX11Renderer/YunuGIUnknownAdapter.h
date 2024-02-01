#pragma once
#include "IUnknown.h"
#include "IRenderableObject.h"

namespace yunuGIAdapter
{
	class IUnknownAdapter : public yunuGI::IUnknown
	{
	public:
		IUnknownAdapter(IUnknown* obj) 
			: object(obj)
		{
			
		}
		virtual ~IUnknownAdapter() {}
		
	private:
		IUnknown* object;

	};
}