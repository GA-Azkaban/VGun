#pragma once
#include <functional>

#include "Component.h"

namespace HDEngine
{
	class InputSystem;
}

namespace HDData
{
	class ImageUI;

	class HODO_API ToggleUI : public Component
	{
	public:
		ToggleUI();

	public:
		void Start() override;
		void Update() override;
		void SetToggleOnEvent(std::function<void()> event);
		void SetToggleOffEvent(std::function<void()> event);
		void CallToggleOnEvent();
		void CallToggleOffEvent();
		void SetSortOrder(float ord);

	public:
		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();

	public:
		void SetOnComp(ImageUI* comp);
		void SetOffComp(ImageUI* comp);
		ImageUI* GetOnComp();
		ImageUI* GetOffComp();
		bool GetIsOn();

	private:
		ImageUI* _toggleOn;
		ImageUI* _toggleOff;

		bool _isToggleOn;

		std::function<void()> _toggleOnEvent;
		std::function<void()> _toggleOffEvent;
	};
}





