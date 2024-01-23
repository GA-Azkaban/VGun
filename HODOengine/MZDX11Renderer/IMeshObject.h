#pragma once
#include "IRenderableObject.h"
#include <vector>
#include <string>
#include <d3d11.h>

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

	virtual void SetMesh(const std::string& fileName) = 0;
	virtual void SetVertexShader(const std::string& fileName) = 0;
	virtual void SetPixelShader(const std::string& fileName) = 0;

private:
	static std::vector<IMeshObject*> meshObjects;
	friend class DeferredRenderer;
};