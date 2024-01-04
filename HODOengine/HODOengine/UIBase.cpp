#include "UIBase.h"
#include "Transform.h"

namespace HDData
{
	UIBase::UIBase()
		:_sketchable(nullptr)
	{

	}

	void UIBase::UpdateRenderData()
	{
		_sketchable->SetWorldTM(GetTransform()->GetWorldTM());
	}
}
