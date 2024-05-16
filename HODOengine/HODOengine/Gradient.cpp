﻿#include "Gradient.h"

namespace HDData
{

	Gradient::Gradient()
	{

	}

	Gradient::~Gradient()
	{

	}

	DirectX::XMINT4 Gradient::Evaluate(float time)
	{
		DirectX::XMINT4 ret{ 1, 1, 1, 1 };

		for (int i = 0; i < colorKeys.size(); ++i)
		{
			if (colorKeys[i].time > time)
			{
				break;
			}
			ret.x = colorKeys[i].color.x;
			ret.y = colorKeys[i].color.y;
			ret.z = colorKeys[i].color.z;
		}

		for (int i = 0; i < alphaKeys.size(); ++i)
		{
			if (alphaKeys[i].time > time)
			{
				break;
			}
			ret.w = alphaKeys[i].alpha;
		}

		return ret;
	}

	void Gradient::SetKeys(std::vector<GradientColorKey> colorKeys, std::vector<GradientAlphaKey> alphaKeys)
	{
		this->colorKeys = colorKeys;
		this->alphaKeys = alphaKeys;
	}

}
