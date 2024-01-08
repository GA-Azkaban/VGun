#include "StaticMeshObject.h"
#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "Mesh.h"
#include "Material.h"
#include "RasterizerState.h"

StaticMeshObject::StaticMeshObject(ModelData modelData, Material* material)
	: m_modelData(modelData), m_material(material), m_isActive(true),
	m_world{ XMMatrixIdentity() }, m_position{ 0, 0, 0 }, m_rotation{ 0, 0, 0, 1 }, m_scale{ 1, 1, 1 }
	//m_meshBox(), m_depth(1.0f), m_isPickingOn(true)
{

}

StaticMeshObject::~StaticMeshObject()
{
	delete m_material;
}

bool StaticMeshObject::Pick(float x, float y)
{
	//   if (!m_isActive)
	//       return false;

	   //if (!m_isPickingOn)
	   //	return false;
	//   // Ray definition in view space.
	//   XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//   XMVECTOR rayDir = XMVectorSet(x, y, 1.0f, 0.0f);

	//   // Tranform ray to local space of Mesh.
	//   XMMATRIX V = MZCamera::GetMainCamera()->View();
	//   XMVECTOR detView = XMMatrixDeterminant(V);
	//   XMMATRIX invView = XMMatrixInverse(&detView, V);

	//   //XMMATRIX W = XMLoadFloat4x4(&m_world);
	//   XMVECTOR detWorld = XMMatrixDeterminant(m_world);
	//   XMMATRIX invWorld = XMMatrixInverse(&detWorld, m_world);

	//   XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	//   rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	//   rayDir = XMVector3TransformNormal(rayDir, toLocal);

	//   // Make the ray direction unit length for the intersection tests.
	//   rayDir = XMVector3Normalize(rayDir);

	//   float tmin = 0.0f;
	//   if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &m_meshBox, &tmin))
	//   {
	//       m_depth = tmin;
	//       return true;
	//   }
	return false;
}

void StaticMeshObject::Update(float deltaTime)
{
	if (!m_isActive)
		return;

	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rotX = XMMatrixRotationX(m_rotation.x);
	XMMATRIX rotY = XMMatrixRotationY(m_rotation.y);
	XMMATRIX rotZ = XMMatrixRotationZ(m_rotation.z);
	XMMATRIX sc = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	m_world = sc * rotZ * rotY * rotX * trans;
}

void StaticMeshObject::Render()
{
	if (!m_isActive)
		return;

	// 이거는 바깥쪽에서 한다.
	//MZDX11Renderer::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//MZDX11Renderer::Instance().GetDeviceContext()->RSSetState(RasterizerState::Instance.Get().GetSolidRS());
	XMMATRIX invWorld = XMMatrixTranspose(m_world);

	XMMATRIX view = MZCamera::GetMainCamera()->View();
	XMMATRIX proj = MZCamera::GetMainCamera()->Proj();
	XMMATRIX worldViewProj = m_world * view * proj;
	XMMATRIX invWVP = XMMatrixTranspose(worldViewProj);

	VertexShader* vertexShader = m_material->GetVertexShader();
	PixelShader* pixelShader = m_material->GetPixelShader();

	vertexShader->SetMatrix4x4("world", invWorld);
	vertexShader->SetMatrix4x4("worldViewProj", invWVP);

	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	pixelShader->SetSamplerState("Sampler", m_material->GetSamplerState());
	pixelShader->SetShaderResourceView("Texture", m_material->GetTextureSRV());
	pixelShader->SetShaderResourceView("NormalMap", m_material->GetNormalMapSRV());

	pixelShader->CopyAllBufferData();
	pixelShader->SetShader();

	/*for (UINT i = 0; i < m_meshes.size(); ++i)
	{
		m_meshes[i]->BindBuffers();
		m_meshes[i]->Draw();
	}*/
	for (UINT i = 0; i < m_modelData.meshes.size(); ++i)
	{
		m_modelData.meshes[i]->BindBuffers();
		m_modelData.meshes[i]->Draw();
	}
}
