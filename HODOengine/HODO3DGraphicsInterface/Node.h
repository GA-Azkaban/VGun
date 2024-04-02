#pragma once
#include "MathHeader.h"
#include <string>
#include <vector>

namespace HDCommon
{
	class Transform;
}

// structure containing bone information
struct Bone
{
	int id = 0;	// position of the bone in final upload array
	DirectX::XMMATRIX offset = DirectX::XMMatrixIdentity();
};

// structure to hold node hierarchy
struct Node
{
	std::string name = "";
	Bone bone;
	HDCommon::Transform* nodeTransform = nullptr;
	//Matrix nodeTransform = DirectX::XMMatrixIdentity();
	Matrix nodeTransformOffset = DirectX::XMMatrixIdentity();
	Matrix rootNodeInvTransform = DirectX::XMMatrixIdentity();
	std::vector<Node> children = {};
};
