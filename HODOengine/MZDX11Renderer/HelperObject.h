#pragma once
#include "DX11Define.h"
#include "IDebugObject.h"

/// <summary>
/// 3D공간에서 축이나 그리드 등의 형태로 표현되어
/// 위치나 공간 크기 등을 보다 쉽게 볼 수 있도록 하는 오브젝트
/// 
/// 2023.07.06 MJKIM
/// </summary>

class Mesh;
class Material;
class VertexShader;
class PixelShader;

class HelperObject : public IDebugObject
{
public:
	HelperObject();
	~HelperObject();

public:
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void SetWorldTM(const XMMATRIX& tm) override;
	virtual void SetActive(bool isActive) override { m_isActive = isActive; };

	void SetMesh(const std::string& meshName);

	void SetPosition(float x, float y, float z) { m_position.x = x; m_position.y = y; m_position.z = z; }
	void SetRotation(float x, float y, float z) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; }
	void SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; }

private:
	//ComPtr<ID3D11DeviceContext> m_deviceContext;
	std::vector<Mesh*> m_meshes;
	Material* m_material;
	bool m_isActive;

	DirectX::XMMATRIX m_world;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT4 m_rotation;
	DirectX::XMFLOAT3 m_scale;
};