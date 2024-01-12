#pragma once
#include "DX11Define.h"
#include "IMeshObject.h"
#include "xnacollision.h"
#include "Node.h"
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
	virtual bool Pick(float x, float y) override;

	virtual void SetWorldTM(const XMMATRIX& tm) override { m_world = tm; };

	virtual void SetMesh(const std::string& fileName) override;
	virtual void SetVertexShader(const std::string& fileName) override;
	virtual void SetPixelShader(const std::string& fileName) override;

	void SetDiffuseTexture(const std::string& fileName);
	void SetNormalTexture(const std::string& fileName);
	void SetSamplerState(ID3D11SamplerState* sampler);

	void PlayAnimation(const std::string& animName);
	void PlayAnimation(UINT index);

	virtual void SetActive(bool isActive) override { m_isActive = isActive; };
	virtual void SetPickingMode(bool isPickingModeOn) { };
	//virtual void SetPickingMode(bool isPickingModeOn) { m_isPickingOn = isPickingModeOn; };

	//XMFLOAT3 GetCenterPos() { return m_meshBox.Center; }
	//XMFLOAT3 GetColliderBoxLength() { return m_meshBox.Extents; }
	//float GetDepth() { return m_depth; }
private:
	void UpdateAnimation(float deltaTime, const Node& node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform);
	std::pair<UINT, float> GetTimeFraction(std::vector<float>& timeStamps, float deltaTime);

private:
	std::vector<Mesh*> m_meshes;
	Material* m_material;
	bool m_isActive;

	std::string m_fileName;
	std::unordered_map<std::string, Animation*> m_animations;
	Animation* m_currentAnimation;
	std::vector<DirectX::XMMATRIX> m_boneTransform;

	// Transform Matrix
	DirectX::XMMATRIX m_world;	// Define transformations from local spaces to world space.

	/// Picking을 위한 bouding box
	/*XNA::AxisAlignedBox m_meshBox;
	float m_depth;
	bool m_isPickingOn;*/
};