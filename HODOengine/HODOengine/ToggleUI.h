#pragma once
#include "UIBase.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"

namespace HDEngine
{
	class InputSystem;
}

namespace HDData
{
	class HODO_API ToggleUI : public UIBase
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

		void SetToggleOnImage(const char* fileName);
		void SetToggleOffImage(const char* fileName);

		bool CheckIsClicked();

	private:
		HDEngine::ISketchableImage* _toggleImage;
		HDEngine::InputSystem& _inputSystem;

		const char* _toggleOnImage;
		const char* _toggleOffImage;


		bool _isToggleOn;
	};
}


