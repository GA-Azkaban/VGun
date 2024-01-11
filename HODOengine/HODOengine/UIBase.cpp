#include "UIBase.h"
#include "Transform.h"

namespace HDData
{
	UIBase::UIBase()
		:_sketchable(NULL)
	{

	}

	void UIBase::UpdateRenderData()
	{
		for (const auto& one : _sketchable)
		{
			one->SetWorldTM(GetTransform()->GetWorldTM());
		}

	}
}
