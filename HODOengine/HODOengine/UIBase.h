﻿#pragma once
#include <vector>

#include "Component.h"
#include "Transform.h"
#include "MathHeader.h"
#include "..\\HODO3DGraphicsInterface\\ISketchable.h"

namespace HDData
{
	class UIBase : public Component
	{
	public:
		UIBase();

	public:
		void UpdateRenderData();
		bool CheckFocus();
		virtual void OnUpdateTransform()
		{
			_sketchable->SetWorldTM(GetTransform()->GetWorldTM());
		}

		void OnEnable() override;
		void OnDisable() override;

		int GetSortOrder() const;
		bool GetIsHovering();
		bool GetIsClicked();
		bool GetIsGrabbing();
		bool IsIgnoreFocused();

		void SetIsHovering(bool isHovering);
		void SetIsClicked(bool isClicked);
		void SetIsGrabbing(bool isGrabbing);
		void SetIsIgnoreFocus(bool isIgnore);

		float GetLeft();
		float GetRight();
		float GetTop();
		float GetBottom();

		virtual void OnClickEvent() {};

		HDEngine::ISketchable* _sketchable;

	private:
		int		_sortOrder;
		bool	_isHovering;
		bool	_isClicked;
		bool	_isGrabbing;
		bool	_ignoreFocus;
	};
}
