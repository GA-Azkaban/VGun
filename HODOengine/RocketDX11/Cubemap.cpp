#include "Cubemap.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeferredBuffers.h"
#include <DirectXMath.h>
using namespace DirectX;

namespace RocketCore::Graphics
{

	Cubemap::Cubemap()
		: m_material(nullptr), m_isActive(true), m_size(50.0f), m_envLightIntensity(1.0f)
	{
		m_mesh = ResourceManager::Instance().GetMeshes("skybox")[0];
		m_material = ResourceManager::Instance().GetMaterials("skybox")[0];
		m_vertexShader = ResourceManager::Instance().GetVertexShader("CubeMapVertexShader.cso");
		m_pixelShader = ResourceManager::Instance().GetPixelShader("CubeMapPixelShader.cso");
	}

	Cubemap::~Cubemap()
	{
		delete m_mesh;
		delete m_material;
		delete m_vertexShader;
		delete m_pixelShader;
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

		m_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(scaleMatrix * cameraTranslate));
		m_vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));

		m_vertexShader->CopyAllBufferData();
		m_vertexShader->SetShader();

		m_pixelShader->SetShaderResourceView("Texture", m_material->GetAlbedoMap());

		m_pixelShader->CopyAllBufferData();
		m_pixelShader->SetShader();

		m_mesh->BindBuffers();
		m_mesh->Draw();
	}

	void Cubemap::LoadMesh(const std::string& meshName)
	{
		m_mesh = ResourceManager::Instance().GetMeshes(meshName)[0];
	}

	void Cubemap::SetVertexShader(const std::string& fileName)
	{
		VertexShader* vs = ResourceManager::Instance().GetVertexShader(fileName);
		m_vertexShader = vs;
	}

	void Cubemap::SetPixelShader(const std::string& fileName)
	{
		PixelShader* ps = ResourceManager::Instance().GetPixelShader(fileName);
		m_pixelShader = ps;
	}

	void Cubemap::LoadCubeMapTexture(const std::string& fileName)
	{
		ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance().GetEnvMapInfo(fileName).cubeMapTexture.shaderResourceView.Get();
		if (!diffuseTex)
			return;
		m_material->SetAlbedoMap(diffuseTex, fileName);
		_deferredBuffers->SetEnvironmentMap(fileName);
	}

}
