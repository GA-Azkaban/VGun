#pragma once
#include "IRenderableObject.h"
#include <vector>

class MZCamera;

class ILightObject : public IRenderableObject
{
public:
	ILightObject()
	{
		lightObjects.push_back(this);
	}

	~ILightObject()
	{
		for (auto iter = lightObjects.begin(); iter != lightObjects.end(); ++iter)
		{
			if (*iter == this)
			{
				lightObjects.erase(iter);
				break;
			}
		}
	}

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

private:
	static std::vector<ILightObject*> lightObjects;
	friend class DeferredRenderer;
};

