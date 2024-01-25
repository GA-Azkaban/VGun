#include "SkinningMeshObject.h"
#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "Mesh.h"
#include "Material.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "ResourceManager.h"
#include "MathHelper.h"
#include "AssimpMathConverter.h"
#include <cmath>

SkinningMeshObject::SkinningMeshObject()
	: m_material(nullptr), m_isActive(true),
	m_world{ XMMatrixIdentity() }, m_currentAnimation(nullptr)
{
	m_material = new Material(
		ResourceManager::Instance.Get().GetVertexShader("DeferredSkeletonMeshVS.cso"), 
		ResourceManager::Instance.Get().GetPixelShader("DeferredSkeletonMeshPS.cso"));
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
		m_currentAnimation->accumulatedTime += deltaTime * m_currentAnimation->ticksPerSecond;
		m_currentAnimation->accumulatedTime = fmod(m_currentAnimation->accumulatedTime, m_currentAnimation->duration);		

		if (!m_currentAnimation->isEnd)
		{
			UpdateAnimation(m_currentAnimation->accumulatedTime, *m_node, m_world, m_node->rootNodeInvTransform);
		}

		if (m_currentAnimation->isLoop == false)
		{
			if (m_currentAnimation->accumulatedTime >= m_currentAnimation->duration - 0.5f)
			{
				m_currentAnimation->isEnd = true;
			}
		}
	}
}

void SkinningMeshObject::Render()
{
	if (!m_isActive)
		return;

	MZDX11Renderer::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	MZDX11Renderer::Instance().GetDeviceContext()->RSSetState(RasterizerState::Instance.Get().GetSolidRS());
	
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

	pixelShader->SetShaderResourceView("Albedo", m_material->GetTextureSRV());
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

	if (nodeAnim != nullptr)
	{
		// calculate interpolated position
		DirectX::XMFLOAT3 position = CalcInterpolatedPosition(animationTime, nodeAnim);
		XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z);

		// calculate interpolated rotation
		DirectX::XMFLOAT4 rotation = CalcInterpolatedRotation(animationTime, nodeAnim);
		DirectX::XMVECTOR r = XMLoadFloat4(&rotation);
		DirectX::XMMATRIX rot = XMMatrixRotationQuaternion(r);

		DirectX::XMFLOAT3 scale = CalcInterpolatedScaling(animationTime, nodeAnim);
		XMMATRIX sc = XMMatrixScaling(scale.x, scale.y, scale.z);

		_nodeTransform = XMMatrixTranspose(sc * rot * trans);
	}
	DirectX::XMMATRIX globalTransform = parentTransform * _nodeTransform;

	m_boneTransform[node.bone.id] = globalInvTransform * globalTransform * node.bone.offset;

	// update values for children bones
	for (Node child : node.children)
	{
		UpdateAnimation(animationTime, child, globalTransform, globalInvTransform);
	}
}

DirectX::XMFLOAT3 SkinningMeshObject::CalcInterpolatedPosition(float animationTime, NodeAnimation* nodeAnim)
{
	if (nodeAnim->positions.size() == 1)
	{
		return nodeAnim->positions[0];
	}

	UINT positionIndex = 0;
	for (UINT i = 0; i < nodeAnim->positionTimestamps.size() - 1; ++i)
	{
		if (animationTime < nodeAnim->positionTimestamps[i + 1])
		{
			positionIndex = i;
			break;
		}
	}
	UINT nextPositionIndex = positionIndex + 1;
	float deltaTime = nodeAnim->positionTimestamps[nextPositionIndex] - nodeAnim->positionTimestamps[positionIndex];
	float factor = (animationTime - nodeAnim->positionTimestamps[positionIndex]) / deltaTime;

	DirectX::XMVECTOR start = XMLoadFloat3(&(nodeAnim->positions[positionIndex]));
	DirectX::XMVECTOR end = XMLoadFloat3(&(nodeAnim->positions[nextPositionIndex]));
	DirectX::XMVECTOR delta = end - start;

	DirectX::XMFLOAT3 ret;
	XMStoreFloat3(&ret, start + factor * delta);

	return ret;
}

DirectX::XMFLOAT4 SkinningMeshObject::CalcInterpolatedRotation(float animationTime, NodeAnimation* nodeAnim)
{
	if (nodeAnim->rotations.size() == 1)
	{
		return nodeAnim->rotations[0];
	}

	UINT rotationIndex = 0;
	for (UINT i = 0; i < nodeAnim->rotationTimestamps.size() - 1; ++i)
	{
		if (animationTime < nodeAnim->rotationTimestamps[i + 1])
		{
			rotationIndex = i;
			break;
		}
	}
	UINT nextRotationIndex = rotationIndex + 1;
	float deltaTime = nodeAnim->rotationTimestamps[nextRotationIndex] - nodeAnim->rotationTimestamps[rotationIndex];
	float factor = (animationTime - nodeAnim->rotationTimestamps[rotationIndex]) / deltaTime;

	DirectX::XMVECTOR start = XMLoadFloat4(&(nodeAnim->rotations[rotationIndex]));
	DirectX::XMVECTOR end = XMLoadFloat4(&(nodeAnim->rotations[nextRotationIndex]));

	DirectX::XMVECTOR q = XMQuaternionSlerp(start, end, factor);
	DirectX::XMFLOAT4 ret;
	XMStoreFloat4(&ret, q);

	return ret;
}

DirectX::XMFLOAT3 SkinningMeshObject::CalcInterpolatedScaling(float animationTime, NodeAnimation* nodeAnim)
{
	if (nodeAnim->scales.size() == 1)
	{
		return nodeAnim->scales[0];
	}

	UINT scaleIndex = 0;
	for (UINT i = 0; i < nodeAnim->scaleTimestamps.size() - 1; ++i)
	{
		if (animationTime < nodeAnim->scaleTimestamps[i + 1])
		{
			scaleIndex = i;
			break;
		}
	}
	UINT nextScaleIndex = scaleIndex + 1;
	float deltaTime = nodeAnim->scaleTimestamps[nextScaleIndex] - nodeAnim->scaleTimestamps[scaleIndex];
	float factor = (animationTime - nodeAnim->scaleTimestamps[scaleIndex]) / deltaTime;

	DirectX::XMVECTOR start = XMLoadFloat3(&(nodeAnim->scales[scaleIndex]));
	DirectX::XMVECTOR end = XMLoadFloat3(&(nodeAnim->scales[nextScaleIndex]));
	DirectX::XMVECTOR delta = end - start;

	DirectX::XMFLOAT3 ret;
	XMStoreFloat3(&ret, start + factor * delta);

	return ret;
}

void SkinningMeshObject::PlayAnimation(const std::string& animName, bool isLoop /*= true*/)
{
	auto animIter = m_animations.find(animName);
	if (animIter != m_animations.end())
	{
		m_currentAnimation = animIter->second;
		m_currentAnimation->isLoop = isLoop;
		m_currentAnimation->accumulatedTime = 0.0f;
		if (m_currentAnimation->isLoop == false)
		{
			m_currentAnimation->isEnd = false;
		}
	}
	m_currentAnimation = nullptr;
}

void SkinningMeshObject::PlayAnimation(UINT index, bool isLoop /*= true*/)
{
	auto animIter = m_animations.begin();
	for (UINT i = 0; i < index; ++i)
	{
		++animIter;
		if (animIter == m_animations.end())
		{
			m_currentAnimation = nullptr;
			return;
		}
	}
	m_currentAnimation = animIter->second;
	m_currentAnimation->isLoop = isLoop;
	if (m_currentAnimation->isLoop == false)
	{
		m_currentAnimation->accumulatedTime = 0.0f;
		m_currentAnimation->isEnd = false;
	}
}

void SkinningMeshObject::SetMesh(const std::string& fileName)
{
	m_meshes = ResourceManager::Instance.Get().GetMeshes(fileName);
	// 일단은 메쉬를 세팅해주면 노드 정보와 애니메이션 정보도 불러와서 세팅해주기로 한다.
	m_node = ResourceManager::Instance.Get().GetNode(fileName);
	m_animations = ResourceManager::Instance.Get().GetAnimations(fileName);
}

void SkinningMeshObject::SetVertexShader(const std::string& fileName)
{
	VertexShader* vs = ResourceManager::Instance.Get().GetVertexShader(fileName);
	if (vs != nullptr)
		m_material->SetVertexShader(vs);
}

void SkinningMeshObject::SetPixelShader(const std::string& fileName)
{
	PixelShader* ps = ResourceManager::Instance.Get().GetPixelShader(fileName);
	if (ps != nullptr)
		m_material->SetPixelShader(ps);
}

void SkinningMeshObject::SetDiffuseTexture(const std::string& fileName)
{
	ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance.Get().GetTexture(fileName);
	m_material->SetTextureSRV(diffuseTex);
}

void SkinningMeshObject::SetNormalTexture(const std::string& fileName)
{
	ID3D11ShaderResourceView* normalTex = ResourceManager::Instance.Get().GetTexture(fileName);
	m_material->SetNormalTexture(normalTex);
}

