#include "Sky.h"
#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "Mesh.h"
#include "Material.h"


Sky::Sky()
	: m_material(nullptr), m_isActive(true)
{
	m_material = new Material(ShaderManager::Instance.Get().GetVertexShader("CubeMapVertexShader.cso"), ShaderManager::Instance.Get().GetPixelShader("CubeMapPixelShader.cso"));
	m_material->SetSamplerState(SamplerState::Instance.Get().GetSamplerState());
}

Sky::~Sky()
{
	delete m_material;
}

void Sky::Update(float deltaTime)
{

}

void Sky::Render()
{
	if (!m_isActive)
		return;

	// 이거는 바깥쪽에서 한다.
	//MZDX11Renderer::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//MZDX11Renderer::Instance().GetDeviceContext()->RSSetState(RasterizerState::Instance.Get().GetSolidRS());
	XMFLOAT3 cameraPos = MZCamera::GetMainCamera()->GetPosition();
	XMMATRIX cameraTranslate = XMMatrixTranslation(cameraPos.x, cameraPos.y, cameraPos.z);

	XMMATRIX view = MZCamera::GetMainCamera()->View();
	XMMATRIX proj = MZCamera::GetMainCamera()->Proj();
	XMMATRIX worldViewProj = cameraTranslate * view * proj;
	XMMATRIX invWVP = XMMatrixTranspose(worldViewProj);

	VertexShader* vertexShader = m_material->GetVertexShader();
	PixelShader* pixelShader = m_material->GetPixelShader();

	vertexShader->SetMatrix4x4("worldViewProj", invWVP);

	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	pixelShader->SetSamplerState("Sampler", m_material->GetSamplerState());
	pixelShader->SetShaderResourceView("Texture", m_material->GetTextureSRV());

	pixelShader->CopyAllBufferData();
	pixelShader->SetShader();

	for (UINT i = 0; i < m_meshes.size(); ++i)
	{
		m_meshes[i]->BindBuffers();
		m_meshes[i]->Draw();
	}
}

void Sky::SetMesh(const std::string& meshName)
{
	m_meshes = ResourceManager::Instance.Get().GetMeshes(meshName);
}

void Sky::SetVertexShader(const std::string& fileName)
{
	VertexShader* vs = ShaderManager::Instance.Get().GetVertexShader(fileName);
	m_material->SetVertexShader(vs);
}

void Sky::SetPixelShader(const std::string& fileName)
{
	PixelShader* ps = ShaderManager::Instance.Get().GetPixelShader(fileName);
	m_material->SetPixelShader(ps);
}

void Sky::SetCubeMapTexture(const std::string& fileName)
{
	ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance.Get().GetTexture(fileName);
	m_material->SetTextureSRV(diffuseTex);
}

void Sky::SetSamplerState(ID3D11SamplerState* sampler)
{
	m_material->SetSamplerState(sampler);
}
