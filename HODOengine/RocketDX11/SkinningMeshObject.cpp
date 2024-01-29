#include "SkinningMeshObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ResourceManager.h"
#include "GraphicsStruct.h"
#include "MathHelper.h"
#include "AssimpMathConverter.h"
#include <cmath>
using namespace DirectX;

namespace RocketCore::Graphics
{
	SkinningMeshObject::SkinningMeshObject()
		: m_material(nullptr), m_isActive(true),
		m_world{ XMMatrixIdentity() }, m_currentAnimation(nullptr)
	{
		m_material = new Material(ResourceManager::Instance().GetVertexShader("SkeletonVertexShader.cso"), ResourceManager::Instance().GetPixelShader("SkeletonPixelShader.cso"));
		m_material->SetSamplerState(ResourceManager::Instance().GetSamplerState(ResourceManager::eSamplerState::DEFAULT));
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
				UpdateAnimation(m_currentAnimation->accumulatedTime, *m_node, m_world, m_node->rootNodeInvTransform * m_world);
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

		// 이거는 바깥쪽에서 한번만 하도록 한다면..?
		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(ResourceManager::Instance().GetRenderState(ResourceManager::eRenderState::SOLID));
		
		XMMATRIX invWorld = XMMatrixTranspose(m_world);

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		XMMATRIX worldViewProj = m_world * view * proj;
		XMMATRIX invWVP = XMMatrixTranspose(worldViewProj);

		VertexShader* vertexShader = m_material->GetVertexShader();
		PixelShader* pixelShader = m_material->GetPixelShader();

		vertexShader->SetMatrix4x4("world", invWorld);
		vertexShader->SetMatrix4x4("worldViewProj", invWVP);
		vertexShader->SetMatrix4x4Array("boneTransforms", &m_boneTransform[0], m_boneTransform.size());

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

	void SkinningMeshObject::SetWorldTM(const Matrix& worldTM)
	{
		m_world = worldTM;
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

	void SkinningMeshObject::LoadMesh(const std::string& fileName)
	{
		m_meshes = ResourceManager::Instance().GetMeshes(fileName);
		// 일단은 메쉬를 세팅해주면 노드 정보와 애니메이션 정보도 불러와서 세팅해주기로 한다.
		m_node = ResourceManager::Instance().GetNode(fileName);
		m_animations = ResourceManager::Instance().GetAnimations(fileName);
	}

	void SkinningMeshObject::LoadDiffuseMap(const std::string& fileName)
	{
		ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance().GetTexture(fileName);
		m_material->SetTextureSRV(diffuseTex);
	}

	void SkinningMeshObject::LoadNormalMap(const std::string& fileName)
	{
		ID3D11ShaderResourceView* normalTex = ResourceManager::Instance().GetTexture(fileName);
		m_material->SetNormalTexture(normalTex);
	}

	void SkinningMeshObject::SetSamplerState(ID3D11SamplerState* sampler)
	{
		m_material->SetSamplerState(sampler);
	}
}
