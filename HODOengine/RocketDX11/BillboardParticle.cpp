#include "BillboardParticle.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "GraphicsStruct.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"
#include <vector>
#include <cmath>
using namespace DirectX;

namespace RocketCore::Graphics
{

	BillboardParticle::BillboardParticle()
		: _world(XMMatrixIdentity()), _isActive(true)
	{
		_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);

		std::vector<Vertex> vertices(6);
		std::vector<UINT> indices(6);

		Vertex v1(XMFLOAT3{ -1, 1, 0 }, XMFLOAT2{ 0, 0 });
		vertices[0] = v1;
		Vertex v2(XMFLOAT3{ 1, 1, 0 }, XMFLOAT2{ 1, 0 });
		vertices[1] = v2;
		Vertex v3(XMFLOAT3{ -1, -1, 0 }, XMFLOAT2{ 0, 1 });
		vertices[2] = v3;
		Vertex v4(XMFLOAT3{ -1, -1, 0 }, XMFLOAT2{ 0, 1 });
		vertices[3] = v4;
		Vertex v5(XMFLOAT3{ 1, 1, 0 }, XMFLOAT2{ 1, 0 });
		vertices[4] = v5;
		Vertex v6(XMFLOAT3{ 1, -1, 0 }, XMFLOAT2{ 1, 1 });
		vertices[5] = v6;

		for (UINT i = 0; i < 6; ++i)
		{
			indices[i] = i;
		}

		_mesh = new Mesh(&vertices[0], 6, &indices[0], 6);
		HDEngine::MaterialDesc desc;
		desc.materialName = "BillboardMat";
		desc.albedo = "T_Sparks_D.png";
		desc.color = { 255, 255, 255, 255 };
		_material = ObjectManager::Instance().CreateMaterial(desc);
		_vertexShader = ResourceManager::Instance().GetVertexShader("BillboardVertexShader.cso");
		_pixelShader = ResourceManager::Instance().GetPixelShader("BillboardPixelShader.cso");

	}

	BillboardParticle::~BillboardParticle()
	{

	}

	void BillboardParticle::Update(float deltaTime)
	{

	}

	void BillboardParticle::Render()
	{
		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(_rasterizerState.Get());

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();

		_world = XMMatrixIdentity();
		//_world = XMMatrixRotationX(90.0f * 0.0174532925f);

		// atan2 함수를 통해 빌보드 모델에 적용될 회전값을 계산한다.
		XMFLOAT3 cameraPosition = Camera::GetMainCamera()->GetPosition();
		float angle = std::atan2(0.0 - cameraPosition.x, 0.0 - cameraPosition.z) * (180.0f / 3.141592f);
		float rotation = angle * 0.0174532925f;

		_world = _world * XMMatrixRotationY(rotation);

		_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(_world));
		_vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));
		_vertexShader->SetInt2("tiling", XMINT2(1, 1));

		_vertexShader->CopyAllBufferData();
		_vertexShader->SetShader();

		_pixelShader->SetShaderResourceView("Albedo", _material->GetAlbedoMap());
		_pixelShader->SetFloat4("albedoColor", _material->GetColorFloat4());

		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		_mesh->BindBuffers();
		_mesh->Draw();
	}

}
