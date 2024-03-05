#pragma once
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
		void OnClickEvent(); 

	public:
		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();

	public:
		void SetOnComp(ImageUI* comp);
		void SetOffComp(ImageUI* comp);

	private:
		ImageUI* _toggleOn;
		ImageUI* _toggleOff;

		bool _isToggleOn;
	};
}





