﻿#include "Cubemap.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "DeferredBuffers.h"
#include <DirectXMath.h>
using namespace DirectX;

namespace RocketCore::Graphics
{
	
	Cubemap::Cubemap()
		: m_material(nullptr), m_isActive(true), m_size(50.0f), m_envLightIntensity(1.0f)
	{
		m_material = new Material(ResourceManager::Instance().GetVertexShader("CubeMapVertexShader.cso"), ResourceManager::Instance().GetPixelShader("CubeMapPixelShader.cso"));
		m_meshes = ResourceManager::Instance().GetMeshes("skybox");
	}

	Cubemap::~Cubemap()
	{
		delete m_material;
	}

	Cubemap& Cubemap::Instance()
	{
		static Cubemap instance;
		return instance;
	}

	void Cubemap::Update(float deltaTime)
	{

	}

	void Cubemap::Render()
	{
		if (!m_isActive)
			return;

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::CUBEMAP));
		
		XMFLOAT3 cameraPos = Camera::GetMainCamera()->GetPosition();
		XMMATRIX cameraTranslate = XMMatrixTranslation(cameraPos.x, cameraPos.y, cameraPos.z);

		XMMATRIX scaleMatrix = XMMatrixScaling(m_size, m_size, m_size);

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();

		VertexShader* vertexShader = m_material->GetVertexShader();
		PixelShader* pixelShader = m_material->GetPixelShader();

		vertexShader->SetMatrix4x4("world", XMMatrixTranspose(scaleMatrix * cameraTranslate));
		vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));

		vertexShader->CopyAllBufferData();
		vertexShader->SetShader();

		pixelShader->SetShaderResourceView("Texture", m_material->GetAlbedoMap());

		pixelShader->CopyAllBufferData();
		pixelShader->SetShader();

		for (UINT i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i]->BindBuffers();
			m_meshes[i]->Draw();
		}
	}

	void Cubemap::LoadMesh(const std::string& meshName)
	{
		m_meshes = ResourceManager::Instance().GetMeshes(meshName);
	}

	void Cubemap::LoadVertexShader(const std::string& fileName)
	{
		VertexShader* vs = ResourceManager::Instance().GetVertexShader(fileName);
		m_material->SetVertexShader(vs);
	}

	void Cubemap::LoadPixelShader(const std::string& fileName)
	{
		PixelShader* ps = ResourceManager::Instance().GetPixelShader(fileName);
		m_material->SetPixelShader(ps);
	}

	void Cubemap::LoadCubeMapTexture(const std::string& fileName)
	{
		ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance().GetEnvMapInfo(fileName).cubeMapTexture.shaderResourceView.Get();
		m_material->SetAlbedoMap(diffuseTex);
		_deferredBuffers->SetEnvironmentMap(fileName);
	}

}
