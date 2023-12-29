#pragma once
#include "IRenderableObject.h"
#include <vector>

class IUIObject : public IRenderableObject
{
public:
	IUIObject()
	{
		uiObjects.emplace_back(this);
	}
	virtual ~IUIObject()
	{
		for (auto iter = uiObjects.begin(); iter != uiObjects.end(); ++iter)
		{
			if (*iter == this)
			{
				uiObjects.erase(iter);
				break;
			}
		}
	};

public:
	virtual void Update(float deltaTime) abstract;
	virtual void Render() abstract;
	virtual bool Pick(float x, float y) abstract;
	virtual void SetPickingMode(bool isPickingModeOn) abstract;
	virtual float GetDepth() abstract;
	virtual void SetScreenSpace() abstract;
	virtual void SetWorldSpace() abstract;

private:
	static std::vector<IUIObject*> uiObjects;
	friend class DeferredRenderer;
};