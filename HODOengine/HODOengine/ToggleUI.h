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

	public:
		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();

	private:
		ImageUI* _toggleOn;
		ImageUI* _toggleOff;

		bool _isToggleOn;
	};
}


