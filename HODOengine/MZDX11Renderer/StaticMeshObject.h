#pragma once
#include "DX11Define.h"
#include "IMeshObject.h"
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

	virtual void SetWorldTM(const XMMATRIX& tm) override { m_world = tm; };
	
	virtual void SetMesh(const std::string& fileName) override;
	virtual void SetVertexShader(const std::string& fileName) override;
	virtual void SetPixelShader(const std::string& fileName) override;

	void SetDiffuseTexture(const std::string& fileName);
	void SetNormalTexture(const std::string& fileName);
	void SetSamplerState(ID3D11SamplerState* sampler);

	virtual void SetActive(bool isActive) override { m_isActive = isActive; };
	virtual void SetPickingMode(bool isPickingModeOn) { };

private:
	std::vector<Mesh*> m_meshes;
	Material* m_material;
	bool m_isActive;

	std::string m_fileName;

	// Transform Matrix
	DirectX::XMMATRIX m_world;	// Define transformations from local spaces to world space.
};
