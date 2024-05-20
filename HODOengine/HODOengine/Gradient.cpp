#include "Gradient.h"

namespace HDData
{

	Gradient::Gradient()
	{
		std::vector<GradientColorKey> ck;
		GradientColorKey colorkey1;
		colorkey1.color = { 255, 255, 255 };
		colorkey1.time = 0.0f;
		ck.push_back(colorkey1);
		GradientColorKey colorkey2;
		colorkey2.color = { 255, 255, 255 };
		colorkey2.time = 1.0f;
		ck.push_back(colorkey2);
		std::vector<GradientAlphaKey> ak;
		GradientAlphaKey alphaKey1;
		alphaKey1.alpha = 255;
		alphaKey1.time = 0.0f;
		ak.push_back(alphaKey1);
		GradientAlphaKey alphaKey2;
		alphaKey2.alpha = 255;
		alphaKey2.time = 0.0f;
		ak.push_back(alphaKey2);
		SetKeys(ck, ak);
	}

	Gradient::~Gradient()
	{

	}

	DirectX::XMINT4 Gradient::Evaluate(float time)
	{
		DirectX::XMINT4 ret{ colorKeys[0].color.x, colorKeys[0].color.y, colorKeys[0].color.z, alphaKeys[0].alpha };

		int index = -1;
		for (int i = 0; i < colorKeys.size(); ++i)
		{
			if (colorKeys[i].time > time)
			{
				break;
			}
			index = i;
		}

		if (index >= 0)
		{
			// 더 이상 남은 키가 없을 때는 마지막 키의 컬러값을 출력한다.
			if (index >= colorKeys.size() - 1)
			{
				ret.x = colorKeys[index].color.x;
				ret.y = colorKeys[index].color.y;
				ret.z = colorKeys[index].color.z;
			}
			else
			{
				float totalTime = (colorKeys[index + 1].time - colorKeys[index].time);
				DirectX::XMINT4 deltaColor;
				deltaColor.x = (colorKeys[index + 1].color.x - colorKeys[index].color.x) / totalTime;
				deltaColor.y = (colorKeys[index + 1].color.y - colorKeys[index].color.y) / totalTime;
				deltaColor.z = (colorKeys[index + 1].color.z - colorKeys[index].color.z) / totalTime;
				float deltaTime = (time - colorKeys[index].time);
				deltaColor.x *= static_cast<int>(deltaTime);								
				deltaColor.y *= static_cast<int>(deltaTime);								
				deltaColor.z *= static_cast<int>(deltaTime);
				
				ret.x = colorKeys[index].color.x + deltaColor.x;
				ret.y = colorKeys[index].color.x + deltaColor.y;
				ret.z = colorKeys[index].color.x + deltaColor.z;
			}
		}

		index = -1;
		for (int i = 0; i < alphaKeys.size() - 1; ++i)
		{
			if (alphaKeys[i].time > time)
			{
				break;
			}
			index = i;
		}

		if (index >= 0)
		{
			// 더 이상 남은 키가 없을 때는 마지막 키의 알파값을 출력한다.
			if (index >= alphaKeys.size() - 1)
			{
				ret.w = alphaKeys[index].alpha;
			}
			else
			{
				float totalTime = (alphaKeys[index + 1].time - alphaKeys[index].time);
				int deltaAlpha = (alphaKeys[index + 1].alpha - alphaKeys[index].alpha) / totalTime;
				float deltaTime = (time - alphaKeys[index].time);
				deltaAlpha *= static_cast<int>(deltaTime);

				ret.w = alphaKeys[index].alpha + deltaAlpha;
			}
		}

		return ret;
	}

	void Gradient::SetKeys(std::vector<GradientColorKey> colorKeys, std::vector<GradientAlphaKey> alphaKeys)
	{
		this->colorKeys = colorKeys;
		this->alphaKeys = alphaKeys;
	}

}
