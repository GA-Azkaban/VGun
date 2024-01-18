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
		: m_material(nullptr), m_isActive(true),
		m_world{ XMMatrixIdentity() }
	{
		m_deviceContext = ResourceManager::Instance().GetDeviceContext();
		m_material = new Material(ResourceManager::Instance().GetVertexShader("DebugVertexShader.cso"), ResourceManager::Instance().GetPixelShader("DebugPixelShader.cso"));
		m_RS = ResourceManager::Instance().GetRenderState(ResourceManager::eRenderState::WIREFRAME);
	}

	HelperObject::~HelperObject()
	{
		delete m_material;
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

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		XMMATRIX worldViewProj = m_world * view * proj;
		XMMATRIX invwvp = XMMatrixTranspose(worldViewProj);

		VertexShader* vertexShader = m_material->GetVertexShader();
		PixelShader* pixelShader = m_material->GetPixelShader();

		vertexShader->SetMatrix4x4("worldViewProj", invwvp);

		vertexShader->CopyAllBufferData();
		vertexShader->SetShader();

		pixelShader->CopyAllBufferData();
		pixelShader->SetShader();

		for (UINT i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i]->BindBuffers();
			m_meshes[i]->Draw();
		}
#endif
	}

	void HelperObject::SetWorldTM(const HDMath::HDFLOAT4X4& worldTM)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_world.r[i].m128_f32[j] = worldTM.element[i][j];
			}
		}
	}

	void HelperObject::SetFillModeSolid()
	{
		m_RS = ResourceManager::Instance().GetRenderState(ResourceManager::eRenderState::SOLID);
	}

	void HelperObject::SetFillModeWireframe()
	{
		m_RS = ResourceManager::Instance().GetRenderState(ResourceManager::eRenderState::WIREFRAME);
	}

	void HelperObject::SetColor(const HDMath::HDFLOAT4& color)
	{
		// 보통 헬퍼 오브젝트는 지정된 색깔이 있어서 일단 지금은 색깔 바꾸는 것은 구현 안 한다.
	}

	void HelperObject::SetMesh(const std::string& meshName)
	{
		m_meshes = ResourceManager::Instance().GetMeshes(meshName);
	}
}