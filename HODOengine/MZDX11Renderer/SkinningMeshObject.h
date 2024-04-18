#pragma once
#include "DX11Define.h"
#include "IMeshObject.h"
#include "Animation.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>

class Mesh;
class Material;
class VertexShader;
class PixelShader;

/// <summary>
/// 2023.07.07 MJKIM
/// </summary>
class SkinningMeshObject : public IMeshObject
{
public:
	SkinningMeshObject();
	~SkinningMeshObject();

public:
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	virtual void SetWorldTM(const XMMATRIX& tm) override { m_world = tm; };

	virtual void SetMesh(const std::string& fileName) override;
	virtual void SetVertexShader(const std::string& fileName) override;
	virtual void SetPixelShader(const std::string& fileName) override;

	void SetDiffuseTexture(const std::string& fileName);
	void SetNormalTexture(const std::string& fileName);

	void PlayAnimation(const std::string& animName, bool isLoop = true);
	void PlayAnimation(UINT index, bool isLoop = true);

	virtual void SetActive(bool isActive) override { m_isActive = isActive; };
	
private:
	void UpdateAnimation(float deltaTime, const Node& node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform);
	DirectX::XMFLOAT3 CalcInterpolatedPosition(float animationTime, NodeAnimation* nodeAnim);
	DirectX::XMFLOAT4 CalcInterpolatedRotation(float animationTime, NodeAnimation* nodeAnim);
	DirectX::XMFLOAT3 CalcInterpolatedScaling(float animationTime, NodeAnimation* nodeAnim);

private:
	std::vector<Mesh*> m_meshes;
	Material* m_material;
	bool m_isActive;

	// Animation
	std::unordered_map<std::string, Animation*> m_animations;
	Animation* m_currentAnimation;
	std::vector<DirectX::XMMATRIX> m_boneTransform;
	Node* m_node;	

	// Transform Matrix
	DirectX::XMMATRIX m_world;	// Define transformations from local spaces to world space.
};
