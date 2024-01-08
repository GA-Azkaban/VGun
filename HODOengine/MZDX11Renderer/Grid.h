#pragma once
#include "DX11Define.h"
#include "IDebugObject.h"

/// <summary>
/// 3D공간의 그리드를 출력
/// 
/// 2023.06.29 MJKIM
/// </summary>

class Mesh;
class Material;
class VertexShader;
class PixelShader;


class Grid : public IDebugObject
{
public:
	Grid(ID3D11DeviceContext* deviceContext, ::Mesh* mesh, Material* material);
	~Grid();

public:
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void SetWorldTM(const XMMATRIX& tm) override;
	virtual void SetActive(bool isActive) override { m_isActive = isActive; };

	void SetPosition(float x, float y, float z) { m_position.x = x; m_position.y = y; m_position.z = z; }
	void SetRotation(float x, float y, float z) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; }
	void SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; }

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	::Mesh* m_mesh;
	Material* m_material;
	bool m_isActive;

	DirectX::XMMATRIX m_world;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT4 m_rotation;
	DirectX::XMFLOAT3 m_scale;
};

