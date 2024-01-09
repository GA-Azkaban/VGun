#pragma once
#include "IRenderableObject.h"
#include <vector>
#include <string>

class VertexShader;
class PixelShader;

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
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual bool Pick(float x, float y) = 0;

	virtual void SetMesh(const std::string& fileName) = 0;

	virtual void SetPickingMode(bool isPickingModeOn) = 0;
	virtual float GetDepth() = 0;

private:
	static std::vector<IMeshObject*> meshObjects;
	friend class DeferredRenderer;
};