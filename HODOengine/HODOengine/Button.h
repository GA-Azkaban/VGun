#pragma once
#include <functional>

#include "UIBase.h"

namespace HDData
{
	class Button : public UIBase
	{
	public:
		Button();

	public:
		void SetOnClickEvent(std::function<void()> event);
		std::function<void()> GetClickEvent();

	private:
		std::function<void()> _onClickEvent;

	};
}


