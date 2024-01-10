#pragma once
#include "DX11Define.h"
#include "IMeshObject.h"
#include "xnacollision.h"
#include <vector>

class Mesh;
class Material;
class VertexShader;
class PixelShader;

/// <summary>
/// 2023.07.07 MJKIM
/// </summary>
class StaticMeshObject : public IMeshObject
{
public:
	StaticMeshObject();
	~StaticMeshObject();

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

	virtual void SetActive(bool isActive) override { m_isActive = isActive; };
	virtual void SetPickingMode(bool isPickingModeOn) { };
	//virtual void SetPickingMode(bool isPickingModeOn) { m_isPickingOn = isPickingModeOn; };

	void SetPosition(float x, float y, float z) { m_position.x = x; m_position.y = y; m_position.z = z; }
	void SetRotation(float x, float y, float z) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; }
	void SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; }

	//XMFLOAT3 GetCenterPos() { return m_meshBox.Center; }
	//XMFLOAT3 GetColliderBoxLength() { return m_meshBox.Extents; }
	//float GetDepth() { return m_depth; }

private:
	std::vector<::Mesh*> m_meshes;
	Material* m_material;
	bool m_isActive;

	// Transform Matrix
	DirectX::XMMATRIX m_world;	// Define transformations from local spaces to world space.
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT4 m_rotation;
	DirectX::XMFLOAT3 m_scale;

	/// Picking을 위한 bouding box
	/*XNA::AxisAlignedBox m_meshBox;
	float m_depth;
	bool m_isPickingOn;*/
};
