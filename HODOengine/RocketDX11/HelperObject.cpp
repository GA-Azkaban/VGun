#include "HelperObject.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include "VertexShader.h"
#include "PixelShader.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	HelperObject::HelperObject()
		: m_isActive(true), m_world{ XMMatrixIdentity() }
	{
		m_deviceContext = ResourceManager::Instance().GetDeviceContext();
		m_RS = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::WIREFRAME);
		m_vertexShader = ResourceManager::Instance().GetVertexShader("DebugVertexShader.cso");
		m_pixelShader = ResourceManager::Instance().GetPixelShader("DebugPixelShader.cso");
	}

	HelperObject::~HelperObject()
	{
		
	}

	void HelperObject::Update(float deltaTime)
	{

	}

	void HelperObject::Render()
	{
#ifdef _DEBUG
		if (!m_isActive)
			return;

		m_deviceContext->RSSetState(m_RS.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		m_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(m_world));
		
		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();

		m_vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));

		m_vertexShader->CopyAllBufferData();
		m_vertexShader->SetShader();

		m_pixelShader->CopyAllBufferData();
		m_pixelShader->SetShader();

		for (UINT i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i]->BindBuffers();
			m_meshes[i]->Draw();
		}
#endif
	}

	void HelperObject::SetWorldTM(const Matrix& worldTM)
	{
		m_world = worldTM;
	}

	void HelperObject::SetFillModeSolid()
	{
		m_RS = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
	}

	void HelperObject::SetFillModeWireframe()
	{
		m_RS = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::WIREFRAME);
	}

	void HelperObject::SetColor(const Vector4& color)
	{
		// 보통 헬퍼 오브젝트는 지정된 색깔이 있어서 일단 지금은 색깔 바꾸는 것은 구현 안 한다.
	}

	void HelperObject::LoadMesh(const std::string& meshName)
	{
		m_meshes = ResourceManager::Instance().GetMeshes(meshName);
	}
}
