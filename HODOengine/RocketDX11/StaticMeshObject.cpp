#include "StaticMeshObject.h"
#include "MathHeader.h"
#include "RocketMacroDX11.h"
#include "ResourceManager.h"
#include "GraphicsStruct.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "OutlinePass.h"
#include "VertexShader.h"
#include "PixelShader.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	StaticMeshObject::StaticMeshObject()
		: m_isActive(true), m_receiveTMInfoFlag(false),
		m_world{ XMMatrixIdentity() }
	{
		m_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
		m_vertexShader = ResourceManager::Instance().GetVertexShader("VertexShader.cso");
		m_pixelShader = ResourceManager::Instance().GetPixelShader("PixelShader.cso");
	}

	StaticMeshObject::~StaticMeshObject()
	{

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




		m_materials = ResourceManager::Instance().GetMaterials(fileName);
		m_node = ResourceManager::Instance().GetNode(fileName);
	}

	void StaticMeshObject::LoadMaterial(HDEngine::IMaterial* material, unsigned int element /*= 0*/)
	{
		if (element >= m_materials.size())
			return;

		Material* newMat = dynamic_cast<Material*>(material);
		if (newMat != nullptr)
		{
			m_materials[element] = newMat;
		}
	}

	void StaticMeshObject::LoadAlbedoMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;

		if (fileName == "")
			return;

		m_materials[element]->LoadAlbedoTexture(fileName);
	}

	void StaticMeshObject::LoadNormalMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;

		if (fileName == "")
			return;

		m_materials[element]->LoadNormalTexture(fileName);
	}

	void StaticMeshObject::LoadARMMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;

		if (fileName == "")
			return;

		m_materials[element]->LoadARMTexture(fileName);
	}

	void StaticMeshObject::LoadRoughnessMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;

		if (fileName == "")
			return;

		m_materials[element]->LoadRoughnessTexture(fileName);
	}

	void StaticMeshObject::LoadMetallicMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;

		if (fileName == "")
			return;

		m_materials[element]->LoadMetallicTexture(fileName);
	}

	void StaticMeshObject::SetRoughnessValue(float value, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;
		m_materials[element]->SetRoughnessValue(value);
	}

	void StaticMeshObject::SetMetallicValue(float value, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;
		m_materials[element]->SetMetallicValue(value);
	}

	void StaticMeshObject::SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;
		m_materials[element]->SetColor(r, g, b, a);
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

			m_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(world));

			m_vertexShader->CopyAllBufferData();
			m_vertexShader->SetShader();

			for (UINT i = 0; i < m_meshes.size(); ++i)
			{
				if (m_materials[i]->GetAlbedoMap())
				{
					m_pixelShader->SetInt("useAlbedo", 1);
					m_pixelShader->SetShaderResourceView("Albedo", m_materials[i]->GetAlbedoMap());
					m_pixelShader->SetFloat4("albedoColor", m_materials[i]->GetColorFloat4());
				}
				else
				{
					m_pixelShader->SetInt("useAlbedo", 0);
					m_pixelShader->SetFloat4("albedoColor", m_materials[i]->GetColorFloat4());
				}

				if (m_materials[i]->GetNormalMap())
				{
					m_pixelShader->SetInt("useNormalMap", 1);
					m_pixelShader->SetShaderResourceView("NormalMap", m_materials[i]->GetNormalMap());
				}
				else
				{
					m_pixelShader->SetInt("useNormalMap", 0);
				}

				if (m_materials[i]->GetOcclusionRoughnessMetalMap())
				{
					m_pixelShader->SetInt("useOccMetalRough", 1);
					m_pixelShader->SetShaderResourceView("OcclusionRoughnessMetal", m_materials[i]->GetOcclusionRoughnessMetalMap());
				}
				else
				{
					m_pixelShader->SetInt("useOccMetalRough", 0);
					m_pixelShader->SetFloat("metallicValue", m_materials[i]->GetMetallicValue());
					m_pixelShader->SetFloat("roughnessValue", m_materials[i]->GetRoughnessValue());
				}

				if (m_materials[i]->GetRoughnessMap())
				{
					m_pixelShader->SetInt("useRoughnessMap", 1);
					m_pixelShader->SetShaderResourceView("Roughness", m_materials[i]->GetRoughnessMap());
				}
				else
				{
					m_pixelShader->SetInt("useRoughnessMap", 0);
					m_pixelShader->SetFloat("roughnessValue", m_materials[i]->GetRoughnessValue());
				}

				if (m_materials[i]->GetRoughnessMap())
				{
					m_pixelShader->SetInt("useMetallicMap", 1);
					m_pixelShader->SetShaderResourceView("Metallic", m_materials[i]->GetRoughnessMap());
				}
				else
				{
					m_pixelShader->SetInt("useMetallicMap", 0);
					m_pixelShader->SetFloat("metallicValue", m_materials[i]->GetRoughnessValue());
				}
				
				m_pixelShader->CopyAllBufferData();
				m_pixelShader->SetShader();

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
