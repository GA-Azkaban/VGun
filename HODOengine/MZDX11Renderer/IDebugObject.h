#pragma once
#include "IRenderableObject.h"
#include <vector>

class IDebugObject : public IRenderableObject
{
public:
	IDebugObject()
	{
		debugObjects.emplace_back(this);
	}
	virtual ~IDebugObject()
	{
		for (auto iter = debugObjects.begin(); iter != debugObjects.end(); ++iter)
		{
			if (*iter == this)
			{
				debugObjects.erase(iter);
				break;
			}
		}
	};

public:
	virtual void Update(MZCamera* pCamera, float deltaTime) abstract;
	virtual void RenderToTexture() abstract;
	//virtual bool Pick(MZCamera* pCamera, float x, float y) abstract;

private:
	static std::vector<IDebugObject*> debugObjects;
	friend class DeferredRenderer;
};