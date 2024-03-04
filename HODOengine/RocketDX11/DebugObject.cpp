#include "DebugObject.h"
#include "MathHeader.h"
#include "RocketMacroDX11.h"
#include "ResourceManager.h"
#include "GraphicsStruct.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
using namespace DirectX;

namespace RocketCore::Graphics
{

	DebugObject::DebugObject()
		:m_material(nullptr), m_isActive(true), m_world(XMMatrixIdentity()), m_color(0.0f, 1.0f, 0.0f, 1.0f)
	{
		m_material = new Material(ResourceManager::Instance().GetVertexShader("DebugVertexShader.cso"), ResourceManager::Instance().GetPixelShader("DebugPixelShader.cso"));
		m_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::WIREFRAME);
	}

	DebugObject::~DebugObject()
	{
		delete m_material;
	}

	void DebugObject::SetWorldTM(const Matrix& worldTM)
	{
		m_world = worldTM;
	}

	void DebugObject::SetActive(bool isActive)
	{
		m_isActive = isActive;
	}

	void DebugObject::SetFillModeSolid()
	{
		m_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
	}

	void DebugObject::SetFillModeWireframe()
	{
		m_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::WIREFRAME);
	}

	void DebugObject::SetColor(const Vector4& color)
	{
		m_color = color;
	}

	void DebugObject::LoadMesh(const std::string& fileName)
	{
		m_meshes = ResourceManager::Instance().GetMeshes(fileName);
		m_node = ResourceManager::Instance().GetNode(fileName);
	}

	void DebugObject::Render()
	{
		if (!m_isActive)
			return;

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(m_rasterizerState.Get());

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		XMMATRIX worldViewProj;
		if (m_node != nullptr)
			worldViewProj = m_node->rootNodeInvTransform * m_world * view * proj;
		else
			worldViewProj = m_world * view * proj;
		XMMATRIX wvp = XMMatrixTranspose(worldViewProj);

		VertexShader* vertexShader = m_material->GetVertexShader();
		PixelShader* pixelShader = m_material->GetPixelShader();

		vertexShader->SetMatrix4x4("worldViewProj", wvp);

		vertexShader->CopyAllBufferData();
		vertexShader->SetShader();

		pixelShader->CopyAllBufferData();
		pixelShader->SetShader();

		for (UINT i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i]->BindBuffers();
			m_meshes[i]->Draw();
		}
	}

	DirectX::XMMATRIX DebugObject::GetWorldTM()
	{
		if (m_node != nullptr)
			return m_node->rootNodeInvTransform * m_world;

		return m_world;
	}

}
