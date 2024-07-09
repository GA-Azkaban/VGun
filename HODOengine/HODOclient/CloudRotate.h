#pragma once

#include "..\HODOengine\HODO_API.h"
#include <vector>

namespace HDData
{
	class GameObject;
}

class CloudRotateScript : public HDData::Script
{
public:
	CloudRotateScript() = default;
	CloudRotateScript(std::string name);
	virtual ~CloudRotateScript();

	virtual void Start() override;
	virtual void Update() override;

private:
	HDData::GameObject* _pivotObject = nullptr;
	std::vector<HDData::GameObject*> _childCloud;
	//std::vector<Vector3>* _childPos = nullptr;
	Vector3* _cloudPos = nullptr;
};
