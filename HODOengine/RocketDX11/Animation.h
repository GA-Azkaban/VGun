#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <unordered_map>

// structure containing each node's animation information in one animation
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
	std::string animName = "";
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	float accumulatedTime = 0.0f;
	bool isLoop = true;
	bool isEnd = false;
	std::vector<NodeAnimation*> nodeAnimations;
};


