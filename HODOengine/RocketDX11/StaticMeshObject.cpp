#include "StaticMeshObject.h"
#include "..\\HODOmath\\HODOmath.h"
#include "RocketMacroDX11.h"
#include "ResourceManager.h"
#include "GraphicsStruct.h"
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
		m_material->SetSamplerState(ResourceManager::Instance().GetSamplerState(ResourceManager::eSamplerState::DEFAULT));
		m_renderState = ResourceManager::Instance().GetRenderState(ResourceManager::eRenderState::SOLID);
	}

	StaticMeshObject::~StaticMeshObject()
	{
		delete m_material;
	}

	void StaticMeshObject::SetWorldTM(const HDMath::HDFLOAT4X4& worldTM)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_world.r[i].m128_f32[j] = worldTM.element[i][j];
			}
		}
	}

	void StaticMeshObject::SetActive(bool isActive)
	{
		m_isActive = isActive;
	}

	void StaticMeshObject::SetVertexShader(VertexShader* shader)
	{
		_vertexShader = shader;
	}

	void StaticMeshObject::SetPixelShader(PixelShader* shader)
	{
		_pixelShader = shader;
	}

	void StaticMeshObject::LoadVertexShader(const std::string& fileName)
	{
		VertexShader* vs = ResourceManager::Instance().GetVertexShader(fileName);
		if (vs != nullptr)
			m_material->SetVertexShader(vs);
	}

	void StaticMeshObject::LoadPixelShader(const std::string& fileName)
	{
		PixelShader* ps = ResourceManager::Instance().GetPixelShader(fileName);
		if (ps != nullptr)
			m_material->SetPixelShader(ps);
	}

	void StaticMeshObject::LoadMesh(const std::string& fileName)
	{
		m_meshes = ResourceManager::Instance().GetMeshes(fileName);
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

	void StaticMeshObject::Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{		
		// Grid가 쓰는 Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(_vertexShader->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_pixelShader->GetPixelShader(), nullptr, 0);

		deviceContext->PSSetSamplers(0, 1, _vertexShader->GetAddressOfSampleState());

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(_vertexShader->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBufferType* dataPtr;
		unsigned int bufferNumber;

		DirectX::XMMATRIX w;
		DirectX::XMMATRIX v;
		DirectX::XMMATRIX p;

		w = DirectX::XMMatrixTranspose(m_world);
		v = DirectX::XMMatrixTranspose(view);
		p = DirectX::XMMatrixTranspose(proj);

		HR(deviceContext->Map(_vertexShader->GetMatrixBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		dataPtr = (MatrixBufferType*)mappedResource.pData;

		dataPtr->world = w;
		dataPtr->view = v;
		dataPtr->projection = p;

		deviceContext->Unmap(_vertexShader->GetMatrixBuffer(), 0);

		bufferNumber = 0;

		deviceContext->VSSetConstantBuffers(bufferNumber, 1, _vertexShader->GetAddressOfMatrixBuffer());

		// 렌더스테이트
		deviceContext->RSSetState(m_renderState.Get());

		_model->Render(deviceContext);
	}

	void StaticMeshObject::Render()
	{
		if (!m_isActive)
			return;

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(m_renderState.Get());

		XMMATRIX world = XMMatrixTranspose(m_world);

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		XMMATRIX worldViewProj = m_world * view * proj;
		XMMATRIX wvp = XMMatrixTranspose(worldViewProj);

		VertexShader* vertexShader = m_material->GetVertexShader();
		PixelShader* pixelShader = m_material->GetPixelShader();

		vertexShader->SetMatrix4x4("world", world);
		vertexShader->SetMatrix4x4("worldViewProj", wvp);

		vertexShader->CopyAllBufferData();
		vertexShader->SetShader();

		XMFLOAT3 cameraPos = Camera::GetMainCamera()->GetPosition();
		pixelShader->SetFloat3("cameraPosition", cameraPos);

		pixelShader->SetSamplerState("Sampler", m_material->GetSamplerState());
		pixelShader->SetShaderResourceView("Texture", m_material->GetTextureSRV());
		pixelShader->SetShaderResourceView("NormalMap", m_material->GetNormalMapSRV());

		pixelShader->CopyAllBufferData();
		pixelShader->SetShader();

		for (UINT i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i]->BindBuffers();
			m_meshes[i]->Draw();
		}
	}

	void StaticMeshObject::SetModel(Model* model)
	{
		_model = model;
	}
}