﻿#include "StaticMeshObject.h"
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
	StaticMeshObject::StaticMeshObject()
		: m_material(nullptr), m_isActive(true),
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
		m_material->SetNormalTexture(normalTex);
	}

	void StaticMeshObject::LoadDiffuseMap(const std::string& fileName)
	{
		ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance().GetTexture(fileName);
		m_material->SetTextureSRV(diffuseTex);
	}

	void StaticMeshObject::Render()
	{
		if (!m_isActive)
			return;

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(m_rasterizerState.Get());

		XMMATRIX world = XMMatrixTranspose(m_world);

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		XMMATRIX worldViewProj = m_node->rootNodeInvTransform * m_world * view * proj;
		XMMATRIX wvp = XMMatrixTranspose(worldViewProj);

		VertexShader* vertexShader = m_material->GetVertexShader();
		PixelShader* pixelShader = m_material->GetPixelShader();

		vertexShader->SetMatrix4x4("world", world);
		vertexShader->SetMatrix4x4("worldViewProj", wvp);

		vertexShader->CopyAllBufferData();
		vertexShader->SetShader();

		pixelShader->SetShaderResourceView("Albedo", m_material->GetTextureSRV());
		if (m_material->GetNormalMapSRV())
		{
			pixelShader->SetInt("useNormalMap", 1);
			pixelShader->SetShaderResourceView("NormalMap", m_material->GetNormalMapSRV());
		}
		else
		{
			pixelShader->SetInt("useNormalMap", 0);
		}

		pixelShader->CopyAllBufferData();
		pixelShader->SetShader();

		for (UINT i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i]->BindBuffers();
			m_meshes[i]->Draw();
		}
	}

}
