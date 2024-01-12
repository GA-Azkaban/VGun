#pragma once
#include "DX11Define.h"
#include "ILightObject.h"

class Mesh;
class Material;
class VertexShader;
class PixelShader;

class LightObject : public ILightObject
{
public:
	LightObject(ID3D11DeviceContext* deviceContext, Mesh* mesh, Material* material, DirectX::XMFLOAT3 lightColor);
	~LightObject();

	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	virtual void SetWorldTM(const DirectX::XMMATRIX& tm) override;
	virtual void SetActive(bool isActive) override { m_isActive = isActive; };

	void SetPosition(float x, float y, float z) { m_position.x = x; m_position.y = y; m_position.z = z; }
	void SetRotation(float x, float y, float z) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; }
	void SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; }

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11RasterizerState> m_RS;
	Mesh* m_mesh;
	Material* m_material;
	DirectX::XMFLOAT3 m_lightColor;
	bool m_isActive;

	VertexShader* m_vertexShader;
	PixelShader* m_pixelShader;

	DirectX::XMMATRIX m_world;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT4 m_rotation;
	DirectX::XMFLOAT3 m_scale;
};