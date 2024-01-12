#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <unordered_map>

// structure representing an animation track
//struct BoneTransformTrack
//{
//	std::vector<float> positionTimestamps = {};
//	std::vector<float> rotationTimestamps = {};
//	std::vector<float> scaleTimestamps = {};
//
//	std::vector<DirectX::XMFLOAT3> positions = {};
//	std::vector<DirectX::XMFLOAT4> rotations = {};
//	std::vector<DirectX::XMFLOAT3> scales = {};
//};

// structure to hold bone tree (skeleton)
struct Bone
{
	int id = 0;	// position of the bone in final upload array
	//std::string name = "";
	DirectX::XMMATRIX offset = DirectX::XMMatrixIdentity();
	//std::vector<Bone> children = {};
};

struct NodeAnimation
{
	std::string nodeName;

	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<DirectX::XMFLOAT3> positions = {};
	std::vector<DirectX::XMFLOAT4> rotations = {};
	std::vector<DirectX::XMFLOAT3> scales = {};
};

// structure containing animation information
struct Animation
{
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::vector<NodeAnimation*> nodeAnimations;
};

struct Node
{
	std::string name = "";
	Bone bone;
	DirectX::XMMATRIX nodeTransform = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rootNodeInvTransform = DirectX::XMMatrixIdentity();
	std::vector<Node> children = {};
};

