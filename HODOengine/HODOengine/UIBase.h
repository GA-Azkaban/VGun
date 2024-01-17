#pragma once
#include <vector>

#include "Component.h"
#include "Transform.h"
#include "..\\HODO3DGraphicsInterface\\ISketchable.h"

namespace HDData
{
	class UIBase : public Component
	{
	public:
		UIBase();
		void UpdateRenderData();
		bool CheckFocus();
		virtual void OnUpdateTransform()
		{
			_sketchable->SetWorldTM(GetTransform()->GetWorldTM());
		}

		int GetSortOrder() const;
		bool GetIsFocused();
		bool GetIsClicked();
		bool IsIgnoreFocused();

		float GetLeft();
		float GetRight();
		float GetTop();
		float GetBottom();

		HDEngine::ISketchable* _sketchable;
	private:
		

		int		_sortOrder;
		bool	_isFocused;
		bool	_isClicked;
		bool	_ignoreFocus;
	};
}
