#include "SliderUI.h"
#include "GameObject.h"
#include "ImageUI.h"

namespace HDData
{
	SliderUI::SliderUI()
	{
	}

	void SliderUI::SetActive(bool active)
	{
		_valueText->SetActive(active);
		_sliderBar->SetActive(active);
		_sliderPoint->SetActive(active);
	}

	void SliderUI::SetScreenSpace()
	{
		_valueText->SetScereenSpace();
		_sliderBar->SetScreenSpace();
		_sliderPoint->SetScereenSpace();
	}

	void SliderUI::SetWorldSpace()
	{
		_valueText->SetWorldSpace();
		_sliderBar->SetWorldSpace();
		_sliderPoint->SetWorldSpace();
	}

	void SliderUI::SetIsHorizontal(bool isHorizontal)
	{
		_isHorizontal = isHorizontal;
	}

	bool SliderUI::GetIsHorizontal()
	{
		return _isHorizontal;
	}

	void SliderUI::SetText(const std::string& str)
	{
		_valueText->SetText(str);
	}

	std::string SliderUI::GetText()
	{
		return _valueText->GetText();
	}

	void SliderUI::SetSliderbarImage(const char* fileName)
	{
		_sliderBar->SetImage(fileName);
	}

	void SliderUI::SetSliderpointImage(const char* fileName)
	{
		_sliderPoint->SetImage(fileName);
	}

}
