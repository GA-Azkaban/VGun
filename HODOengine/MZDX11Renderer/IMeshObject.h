#pragma once
#include "IRenderableObject.h"
#include <vector>

class IMeshObject : public IRenderableObject
{
public:
	IMeshObject()
	{
		meshObjects.emplace_back(this);
	}
	virtual ~IMeshObject()
	{
		for (auto iter = meshObjects.begin(); iter != meshObjects.end(); ++iter)
		{
			if (*iter == this)
			{
				meshObjects.erase(iter);
				break;
			}
		}
	};

public:
	virtual void Update(MZCamera* pCamera, float deltaTime) abstract;
	virtual void RenderToTexture() abstract;
	virtual bool Pick(MZCamera* pCamera, float x, float y) abstract;
	virtual void SetPickingMode(bool isPickingModeOn) abstract;
	virtual float GetDepth() abstract;

private:
	static std::vector<IMeshObject*> meshObjects;
	friend class DeferredRenderer;
};