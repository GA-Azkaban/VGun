#include "SkinningMeshObject.h"
#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "Mesh.h"
#include "Node.h"
#include "Material.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "MathHelper.h"
#include "AssimpMathConverter.h"
#include <cmath>

SkinningMeshObject::SkinningMeshObject()
	: m_material(nullptr), m_isActive(true),
	m_world{ XMMatrixIdentity() }, m_currentAnimation(nullptr)
	//m_meshBox(), m_depth(1.0f), m_isPickingOn(true)
{
	m_material = new Material(ShaderManager::Instance.Get().GetVertexShader("SkeletonVertexShader.cso"), ShaderManager::Instance.Get().GetPixelShader("SkeletonPixelShader.cso"));
	m_material->SetSamplerState(SamplerState::Instance.Get().GetSamplerState());
	m_boneTransform.resize(96, XMMatrixIdentity());
}

SkinningMeshObject::~SkinningMeshObject()
{
	delete m_material;
}

void SkinningMeshObject::Update(float deltaTime)
{
	if (m_currentAnimation != nullptr)
	{
		deltaTime = deltaTime * m_currentAnimation->ticksPerSecond / 1000;
		float animationTime = fmod(deltaTime, m_currentAnimation->duration);
		UpdateAnimation(animationTime, m_meshes[0]->GetNode(), DirectX::XMMatrixIdentity(), m_meshes[0]->GetNode().rootNodeInvTransform);
	}
}

void SkinningMeshObject::Render()
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
	vertexShader->SetMatrix4x4Array("boneTransforms", &m_boneTransform[0], m_boneTransform.size());

	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	XMFLOAT3 cameraPos = MZCamera::GetMainCamera()->GetPosition();
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

void SkinningMeshObject::UpdateAnimation(float animationTime, const Node& node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform)
{
	DirectX::XMMATRIX _nodeTransform = (node.nodeTransform);

	NodeAnimation* nodeAnim = nullptr;
	for (UINT i = 0; i < m_currentAnimation->nodeAnimations.size(); ++i)
	{
		if (m_currentAnimation->nodeAnimations[i]->nodeName == node.name)
		{
			nodeAnim = m_currentAnimation->nodeAnimations[i];
			break;
		}
	}
	std::pair<UINT, float> fp;

	if (nodeAnim != nullptr)
	{
		// calculate interpolated position
		DirectX::XMFLOAT3 position{ 0.0f, 0.0f, 0.0f };
		if (nodeAnim->positionTimestamps.size() > 0)
		{
			fp = GetTimeFraction(nodeAnim->positionTimestamps, animationTime);
			DirectX::XMFLOAT3 position1 = nodeAnim->positions[fp.first - 1];
			DirectX::XMFLOAT3 position2 = nodeAnim->positions[fp.first];
			// linear interpolation
			position = MathHelper::Mix(position1, position2, fp.second);
		}

		// calculate interpolated rotation
		DirectX::XMFLOAT4 rotation{ 0.0f, 0.0f, 0.0f, 0.0f };
		if (nodeAnim->rotationTimestamps.size() > 0)
		{
			fp = GetTimeFraction(nodeAnim->rotationTimestamps, animationTime);
			DirectX::XMFLOAT4 rotation1 = nodeAnim->rotations[fp.first - 1];
			DirectX::XMFLOAT4 rotation2 = nodeAnim->rotations[fp.first];
			// linear interpolation
			rotation = MathHelper::Mix(rotation1, rotation2, fp.second);
		}

		DirectX::XMFLOAT3 scale{ 1.0f, 1.0f, 1.0f };
		if (nodeAnim->scaleTimestamps.size() > 0)
		{
			// calculate interpolated scale
			fp = GetTimeFraction(nodeAnim->scaleTimestamps, animationTime);
			DirectX::XMFLOAT3 scale1 = nodeAnim->scales[fp.first - 1];
			DirectX::XMFLOAT3 scale2 = nodeAnim->scales[fp.first];
			// linear interpolation
			DirectX::XMFLOAT3 scale = MathHelper::Mix(scale1, scale2, fp.second);
		}

		XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z);
		XMMATRIX rotX = XMMatrixRotationX(rotation.x);
		XMMATRIX rotY = XMMatrixRotationY(rotation.y);
		XMMATRIX rotZ = XMMatrixRotationZ(rotation.z);
		XMMATRIX sc = XMMatrixScaling(scale.x, scale.y, scale.z);

		_nodeTransform = XMMatrixTranspose(sc * rotZ * rotY * rotX * trans);
	}
	DirectX::XMMATRIX globalTransform = parentTransform * _nodeTransform;

	m_boneTransform[node.bone.id] = globalInvTransform * globalTransform * node.bone.offset;

	// update values for children bones
	for (Node child : node.children)
	{
		UpdateAnimation(animationTime, child, globalTransform, globalInvTransform);
	}
}

std::pair<UINT, float> SkinningMeshObject::GetTimeFraction(std::vector<float>& timeStamps, float deltaTime)
{
	UINT segment = 0;
	while (deltaTime > timeStamps[segment])
		segment++;
	float start = timeStamps[segment - 1];
	float end = timeStamps[segment];
	float frac = (deltaTime - start) / (end - start);
	return { segment, frac };
}

bool SkinningMeshObject::Pick(float x, float y)
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

void SkinningMeshObject::PlayAnimation(const std::string& animName)
{
	auto animIter = m_animations.find(animName);
	if (animIter != m_animations.end())
	{
		m_currentAnimation = animIter->second;
	}
	m_currentAnimation = nullptr;
}

void SkinningMeshObject::PlayAnimation(UINT index)
{
	auto animIter = m_animations.begin();
	for (UINT i = 0; i < index; ++i)
	{
		animIter++;
		if (animIter == m_animations.end())
		{
			m_currentAnimation = nullptr;
			return;
		}
	}
	m_currentAnimation = animIter->second;
}

void SkinningMeshObject::SetMesh(const std::string& fileName)
{
	m_fileName = fileName;
	m_meshes = ResourceManager::Instance.Get().GetLoadedMesh(fileName);
	// 일단은 메쉬를 세팅해주면 애니메이션 정보도 불러와서 세팅해주기로 한다.
	m_animations = ResourceManager::Instance.Get().GetAnimations(fileName);
}

void SkinningMeshObject::SetVertexShader(const std::string& fileName)
{
	VertexShader* vs = ShaderManager::Instance.Get().GetVertexShader(fileName);
	if (vs != nullptr)
		m_material->SetVertexShader(vs);
}

void SkinningMeshObject::SetPixelShader(const std::string& fileName)
{
	PixelShader* ps = ShaderManager::Instance.Get().GetPixelShader(fileName);
	if (ps != nullptr)
		m_material->SetPixelShader(ps);
}

void SkinningMeshObject::SetDiffuseTexture(const std::string& fileName)
{
	ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance.Get().GetLoadedTexture(fileName);
	m_material->SetTextureSRV(diffuseTex);
}

void SkinningMeshObject::SetNormalTexture(const std::string& fileName)
{
	ID3D11ShaderResourceView* normalTex = ResourceManager::Instance.Get().GetLoadedTexture(fileName);
	m_material->SetNormalTexture(normalTex);
}

void SkinningMeshObject::SetSamplerState(ID3D11SamplerState* sampler)
{
	m_material->SetSamplerState(sampler);
}
