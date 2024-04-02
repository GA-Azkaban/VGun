#include "StaticMeshObject.h"
#include "MathHeader.h"
#include "RocketMacroDX11.h"
#include "ResourceManager.h"
#include "GraphicsStruct.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "OutlinePass.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	StaticMeshObject::StaticMeshObject()
		: m_material(nullptr), m_isActive(true), m_receiveTMInfoFlag(false),
		m_world{ XMMatrixIdentity() }
	{
		m_material = new Material(ResourceManager::Instance().GetVertexShader("VertexShader.cso"), ResourceManager::Instance().GetPixelShader("PixelShader.cso"));
		m_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
	}

	StaticMeshObject::~StaticMeshObject()
	{
		delete m_material;
	}

	void StaticMeshObject::SetWorldTM(const Matrix& worldTM)
	{
		m_world = worldTM;
		m_receiveTMInfoFlag = true;
	}

	void StaticMeshObject::SetActive(bool isActive)
	{
		m_isActive = isActive;
	}

	void StaticMeshObject::LoadMesh(const std::string& fileName)
	{
		m_meshes = ResourceManager::Instance().GetMeshes(fileName);
		m_node = ResourceManager::Instance().GetNode(fileName);
	}

	void StaticMeshObject::LoadNormalMap(const std::string& fileName)
	{
		ID3D11ShaderResourceView* normalTex = ResourceManager::Instance().GetTexture(fileName);
		m_material->SetNormalMap(normalTex);
	}

	void StaticMeshObject::LoadAlbedoMap(const std::string& fileName)
	{
		ID3D11ShaderResourceView* albedoTex = ResourceManager::Instance().GetTexture(fileName);
		m_material->SetAlbedoMap(albedoTex);
	}

	void StaticMeshObject::LoadARMMap(const std::string& fileName)
	{
        ID3D11ShaderResourceView* armTex = ResourceManager::Instance().GetTexture(fileName);
        m_material->SetOcclusionRoughnessMetalMap(armTex);
    }

	void StaticMeshObject::LoadRoughnessMap(const std::string& fileName)
	{
		ID3D11ShaderResourceView* roughnessTex = ResourceManager::Instance().GetTexture(fileName);
		m_material->SetOcclusionRoughnessMetalMap(roughnessTex);
	}

	void StaticMeshObject::LoadMetallicMap(const std::string& fileName)
	{
		ID3D11ShaderResourceView* metallicTex = ResourceManager::Instance().GetTexture(fileName);
		m_material->SetOcclusionRoughnessMetalMap(metallicTex);
	}

	void StaticMeshObject::SetRoughnessValue(float value)
	{
		m_material->SetRoughness(value);
	}

	void StaticMeshObject::SetMetallicValue(float value)
	{
		m_material->SetMetallic(value);
	}

	void StaticMeshObject::SetAlbedoColor(UINT r, UINT g, UINT b, UINT a /* = 255 */)
	{
		m_material->SetAlbedoColor(r, g, b, a);
	}

	void StaticMeshObject::SetAlbedoColor(Vector4 color)
	{
		m_material->SetAlbedoColor(color);
	}

	void StaticMeshObject::Render()
	{
		if (!m_isActive)
			return;

		if (m_receiveTMInfoFlag)
		{
			ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			ResourceManager::Instance().GetDeviceContext()->RSSetState(m_rasterizerState.Get());

			XMMATRIX world = m_world;

			if (m_node != nullptr)
				world = m_node->rootNodeInvTransform * m_world;

			VertexShader* vertexShader = m_material->GetVertexShader();
			PixelShader* pixelShader = m_material->GetPixelShader();

			vertexShader->SetMatrix4x4("world", XMMatrixTranspose(world));

			vertexShader->CopyAllBufferData();
			vertexShader->SetShader();
			for (UINT i = 0; i < m_meshes.size(); ++i)
			{
			if (m_material->GetAlbedoMap())
			{
				pixelShader->SetInt("useAlbedo", 1);
				pixelShader->SetShaderResourceView("Albedo", m_material->GetAlbedoMap());
				pixelShader->SetFloat4("albedoColor", m_material->GetAlbedoColor());
			}
			else
			{
				pixelShader->SetInt("useAlbedo", 0);
				pixelShader->SetFloat4("albedoColor", m_material->GetAlbedoColor());
			}

			if (m_material->GetNormalMap())
			{
				pixelShader->SetInt("useNormalMap", 1);
				pixelShader->SetShaderResourceView("NormalMap", m_material->GetNormalMap());
			}
			else
			{
				pixelShader->SetInt("useNormalMap", 0);
			}

			if (m_material->GetOcclusionRoughnessMetalMap())
			{
				pixelShader->SetInt("useOccMetalRough", 1);
				pixelShader->SetShaderResourceView("OcclusionRoughnessMetal", m_material->GetOcclusionRoughnessMetalMap());
			}
			else
			{
				pixelShader->SetInt("useOccMetalRough", 0);
				pixelShader->SetFloat("gMetallic", m_material->GetMetallic());
				pixelShader->SetFloat("gRoughness", m_material->GetRoughness());
			}

			pixelShader->CopyAllBufferData();
			pixelShader->SetShader();

			//for (UINT i = 0; i < m_meshes.size(); ++i)
			//{
				m_meshes[i]->BindBuffers();
				m_meshes[i]->Draw();
			}
		}

		m_receiveTMInfoFlag = false;
	}

	DirectX::XMMATRIX StaticMeshObject::GetWorldTM()
	{
		if (m_node != nullptr)
			return m_node->rootNodeInvTransform * m_world;

		return m_world;
	}

}
