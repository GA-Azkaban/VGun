#include "SkinningMeshObject.h"
#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "Mesh.h"
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
	//m_world.r[3].m128_f32[1] = -5.0f;
}

SkinningMeshObject::~SkinningMeshObject()
{
	delete m_material;
}

void SkinningMeshObject::Update(float deltaTime)
{
	//float ticks = (float)GetTickCount();
	if (m_currentAnimation != nullptr)
	{
		//float dt = ticks * m_currentAnimation->ticksPerSecond / 1000;
		//float animationTime = fmod(dt, m_currentAnimation->duration);
		m_currentAnimation->accumulatedTime += deltaTime * m_currentAnimation->ticksPerSecond;
		/*if (m_animationTime >= m_currentAnimation->duration)
		{
			int a = 0;
		}*/
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

	/*if (nextPositionIndex == nodeAnim->positionTimestamps.size() - 1)
	{
		if (factor >= 0.98f)
		{
			loopCount--;
			if (loopCount == 0)
			{
				m_isAnimEnd = true;
				return nodeAnim->positions[nextPositionIndex];
			}
		}
	}*/


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

	/*if (nextRotationIndex == nodeAnim->rotationTimestamps.size() - 1)
	{
		if (factor >= 0.98f)
		{
			loopCount--;
			if (loopCount == 0)
			{
				m_isAnimEnd = true;
				return nodeAnim->rotations[nextRotationIndex];
			}
		}
	}*/

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

	/*if (nextScaleIndex == nodeAnim->scaleTimestamps.size() - 1)
	{
		if (factor >= 0.98f)
		{
			loopCount--;
			if (loopCount == 0)
			{
				m_isAnimEnd = true;
				return nodeAnim->scales[nextScaleIndex];
			}
		}
	}*/

	DirectX::XMVECTOR start = XMLoadFloat3(&(nodeAnim->scales[scaleIndex]));
	DirectX::XMVECTOR end = XMLoadFloat3(&(nodeAnim->scales[nextScaleIndex]));
	DirectX::XMVECTOR delta = end - start;

	DirectX::XMFLOAT3 ret;
	XMStoreFloat3(&ret, start + factor * delta);

	return ret;
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

void SkinningMeshObject::PlayAnimation(const std::string& animName, bool isLoop /*= true*/)
{
	auto animIter = m_animations.find(animName);
	if (animIter != m_animations.end())
	{
		m_currentAnimation = animIter->second;
		m_currentAnimation->isLoop = isLoop;
		if (m_currentAnimation->isLoop == false)
		{
			m_currentAnimation->accumulatedTime = 0.0f;
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
	ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance.Get().GetTexture(fileName);
	m_material->SetTextureSRV(diffuseTex);
}

void SkinningMeshObject::SetNormalTexture(const std::string& fileName)
{
	ID3D11ShaderResourceView* normalTex = ResourceManager::Instance.Get().GetTexture(fileName);
	m_material->SetNormalTexture(normalTex);
}

void SkinningMeshObject::SetSamplerState(ID3D11SamplerState* sampler)
{
	m_material->SetSamplerState(sampler);
}
