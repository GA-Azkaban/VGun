#include "SkinningMeshObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "GraphicsStruct.h"
#include "OutlinePass.h"
#include "MathHelper.h"
#include "AssimpMathConverter.h"
#include <cmath>
#include <algorithm>
using namespace DirectX;

namespace RocketCore::Graphics
{
	SkinningMeshObject::SkinningMeshObject()
		: m_material(nullptr), m_isActive(true), m_receiveTMInfoFlag(false),
		m_world{ XMMatrixIdentity() }, m_currentAnimation(nullptr),
		m_blendFlag(false), m_isOutlineActive(false)
	{
		m_material = new Material(ResourceManager::Instance().GetVertexShader("SkeletonVertexShader.cso"), ResourceManager::Instance().GetPixelShader("SkeletonPixelShader.cso"));
		m_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
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
			if (m_blendFlag)
			{
				m_currentAnimation->accumulatedTime += deltaTime * m_currentAnimation->ticksPerSecond;

				if (m_currentAnimation->accumulatedTime > m_currentAnimation->blendDuration)
				{
					m_blendFlag = false;
					m_currentAnimation->accumulatedTime = 0.0f;
					return;
				}

				UpdateBlendAnimation(m_previousAnimation->accumulatedTime, m_currentAnimation->accumulatedTime, *m_node, m_world, DirectX::XMMatrixInverse(nullptr, m_world));
			}
			else
			{
				m_currentAnimation->accumulatedTime += deltaTime * m_currentAnimation->ticksPerSecond;

				if (m_currentAnimation->accumulatedTime > m_currentAnimation->duration)
				{
					m_currentAnimation->accumulatedTime = 0.0f;
					m_currentAnimation->isEnd = true;
					return;
				}

				if (m_currentAnimation->isLoop == true)
				{
					if (m_currentAnimation->isEnd == true)
					{
						m_currentAnimation->isEnd = false;
					}
				}

				if (!m_currentAnimation->isEnd)
				{
					UpdateAnimation(m_currentAnimation->accumulatedTime, *m_node, m_world, DirectX::XMMatrixInverse(nullptr, m_world));
				}
			}
		}
	}

	void SkinningMeshObject::Render()
	{
		if (!m_isActive)
			return;

		if (m_receiveTMInfoFlag)
		{
			ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			ResourceManager::Instance().GetDeviceContext()->RSSetState(m_rasterizerState.Get());

			XMMATRIX world = m_node->rootNodeInvTransform * m_world;

			VertexShader* vertexShader = m_material->GetVertexShader();
			PixelShader* pixelShader = m_material->GetPixelShader();

			vertexShader->SetMatrix4x4("world", XMMatrixTranspose(world));
			vertexShader->SetMatrix4x4Array("boneTransforms", &m_boneTransform[0], m_boneTransform.size());

			vertexShader->CopyAllBufferData();
			vertexShader->SetShader();

			if (m_material->GetAlbedoMap())
			{
				pixelShader->SetInt("useAlbedo", 1);
				pixelShader->SetShaderResourceView("Albedo", m_material->GetAlbedoMap());
			}
			else
			{
				pixelShader->SetInt("useAlbedo", 0);
			}

			if (m_material->GetNormalMap())
			{
				pixelShader->SetInt("useNormalMap", 1);
				pixelShader->SetShaderResourceView("NormalMap", m_material->GetNormalMap());
			}
			else
			{
				pixelShader->SetInt("useNormalMap", 0);
			}

			if (m_material->GetOcclusionRoughnessMetalMap())
			{
				pixelShader->SetInt("useOccMetalRough", 1);
				pixelShader->SetShaderResourceView("OcclusionRoughnessMetal", m_material->GetOcclusionRoughnessMetalMap());
			}
			else
			{
				pixelShader->SetInt("useOccMetalRough", 0);
				pixelShader->SetInt("gMetallic", m_material->GetMetallic());
				pixelShader->SetInt("gRoughness", m_material->GetRoughness());
			}

			pixelShader->CopyAllBufferData();
			pixelShader->SetShader();

			for (UINT i = 0; i < m_meshes.size(); ++i)
			{
				m_meshes[i]->BindBuffers();
				m_meshes[i]->Draw();
			}
		}

		m_receiveTMInfoFlag = false;
	}

	void SkinningMeshObject::SetWorldTM(const Matrix& worldTM)
	{
		m_world = worldTM;
		m_receiveTMInfoFlag = true;
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

	void SkinningMeshObject::UpdateBlendAnimation(float prevAnimationTime, float animationTime, const Node& node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform)
	{
		DirectX::XMMATRIX _nodeTransform = (node.nodeTransform);

		NodeAnimation* prevAnim = nullptr;
		for (UINT i = 0; i < m_previousAnimation->nodeAnimations.size(); ++i)
		{
			if (m_previousAnimation->nodeAnimations[i]->nodeName == node.name)
			{
				prevAnim = m_previousAnimation->nodeAnimations[i];
				break;
			}
		}

		NodeAnimation* currAnim = nullptr;
		for (UINT i = 0; i < m_currentAnimation->nodeAnimations.size(); ++i)
		{
			if (m_currentAnimation->nodeAnimations[i]->nodeName == node.name)
			{
				currAnim = m_currentAnimation->nodeAnimations[i];
				break;
			}
		}

		if (prevAnim != nullptr && currAnim != nullptr)
		{
			// calculate interpolated position
			DirectX::XMFLOAT3 position = CalcBlendedPosition(prevAnimationTime, animationTime, m_currentAnimation->blendDuration, prevAnim, currAnim);
			XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z);

			// calculate interpolated rotation
			DirectX::XMFLOAT4 rotation = CalcBlendedRotation(prevAnimationTime, animationTime, m_currentAnimation->blendDuration, prevAnim, currAnim);
			DirectX::XMVECTOR r = XMLoadFloat4(&rotation);
			DirectX::XMMATRIX rot = XMMatrixRotationQuaternion(r);

			DirectX::XMFLOAT3 scale = CalcBlendedScaling(prevAnimationTime, animationTime, m_currentAnimation->blendDuration, prevAnim, currAnim);
			XMMATRIX sc = XMMatrixScaling(scale.x, scale.y, scale.z);

			_nodeTransform = XMMatrixTranspose(sc * rot * trans);
		}
		DirectX::XMMATRIX globalTransform = parentTransform * _nodeTransform;

		m_boneTransform[node.bone.id] = globalInvTransform * globalTransform * node.bone.offset;

		// update values for children bones
		for (Node child : node.children)
		{
			UpdateBlendAnimation(prevAnimationTime, animationTime, child, globalTransform, globalInvTransform);
		}
	}

	DirectX::XMFLOAT3 SkinningMeshObject::CalcBlendedPosition(float prevAnimationTime, float currAnimationTime, float blendDuration, NodeAnimation* prevAnim, NodeAnimation* currentAnim)
	{
		UINT positionIndex = 0;
		for (UINT i = 0; i < prevAnim->positionTimestamps.size() - 1; ++i)
		{
			if (prevAnimationTime < prevAnim->positionTimestamps[i + 1])
			{
				positionIndex = i;
				break;
			}
		}

		UINT nextPositionIndex = 0;

		float factor = currAnimationTime / blendDuration;

		DirectX::XMVECTOR start = XMLoadFloat3(&(prevAnim->positions[positionIndex]));
		DirectX::XMVECTOR end = XMLoadFloat3(&(currentAnim->positions[nextPositionIndex]));
		DirectX::XMVECTOR delta = end - start;

		DirectX::XMFLOAT3 ret;
		XMStoreFloat3(&ret, start + factor * delta);

		return ret;
	}

	DirectX::XMFLOAT4 SkinningMeshObject::CalcBlendedRotation(float prevAnimationTime, float currAnimationTime, float blendDuration, NodeAnimation* prevAnim, NodeAnimation* currentAnim)
	{
		UINT rotationIndex = 0;
		for (UINT i = 0; i < prevAnim->rotationTimestamps.size() - 1; ++i)
		{
			if (prevAnimationTime < prevAnim->rotationTimestamps[i + 1])
			{
				rotationIndex = i;
				break;
			}
		}
		UINT nextRotationIndex = 0;

		float factor = currAnimationTime / blendDuration;

		DirectX::XMVECTOR start = XMLoadFloat4(&(prevAnim->rotations[rotationIndex]));
		DirectX::XMVECTOR end = XMLoadFloat4(&(currentAnim->rotations[nextRotationIndex]));

		DirectX::XMVECTOR q = XMQuaternionSlerp(start, end, factor);
		DirectX::XMFLOAT4 ret;
		XMStoreFloat4(&ret, q);

		return ret;
	}

	DirectX::XMFLOAT3 SkinningMeshObject::CalcBlendedScaling(float prevAnimationTime, float currAnimationTime, float blendDuration, NodeAnimation* prevAnim, NodeAnimation* currentAnim)
	{
		UINT scaleIndex = 0;
		for (UINT i = 0; i < prevAnim->scaleTimestamps.size() - 1; ++i)
		{
			if (prevAnimationTime < prevAnim->scaleTimestamps[i + 1])
			{
				scaleIndex = i;
				break;
			}
		}
		UINT nextScaleIndex = 0;

		float factor = currAnimationTime / blendDuration;

		DirectX::XMVECTOR start = XMLoadFloat3(&(prevAnim->scales[scaleIndex]));
		DirectX::XMVECTOR end = XMLoadFloat3(&(currentAnim->scales[nextScaleIndex]));
		DirectX::XMVECTOR delta = end - start;

		DirectX::XMFLOAT3 ret;
		XMStoreFloat3(&ret, start + factor * delta);

		return ret;
	}

	void SkinningMeshObject::PlayAnimation(const std::string& fileName, bool isLoop /*= true*/)
	{
		LoadMesh(fileName);
		//PlayAnimation(0, isLoop);
		auto animIter = m_animations.find(fileName);
		if (animIter == m_animations.end())
		{
			m_currentAnimation = nullptr;
			return;
		}

		if (m_currentAnimation == &(animIter->second))
			return;

		m_previousAnimation = m_currentAnimation;
		m_currentAnimation = &(animIter->second);
		m_currentAnimation->isLoop = isLoop;

		if (m_previousAnimation != nullptr)
		{
			if (m_previousAnimation->uniqueAnimNum != m_currentAnimation->uniqueAnimNum)
			{
				m_currentAnimation->accumulatedTime = 0.0f;
				if (m_currentAnimation->isLoop == false)
				{
					m_currentAnimation->isEnd = false;
				}
				m_blendFlag = true;
			}
		}
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

		if (m_currentAnimation == &(animIter->second))
			return;

		m_previousAnimation = m_currentAnimation;
		m_currentAnimation = &(animIter->second);
		m_currentAnimation->isLoop = isLoop;

		if (m_previousAnimation != nullptr)
		{
			if (m_previousAnimation->uniqueAnimNum != m_currentAnimation->uniqueAnimNum)
			{
				m_currentAnimation->accumulatedTime = 0.0f;
				if (m_currentAnimation->isLoop == false)
				{
					m_currentAnimation->isEnd = false;
				}
				m_blendFlag = true;
			}
		}

	}

	void SkinningMeshObject::LoadMesh(const std::string& fileName)
	{
		m_meshes = ResourceManager::Instance().GetMeshes(fileName);
		// 일단은 메쉬를 세팅해주면 노드 정보와 애니메이션 정보도 불러와서 세팅해주기로 한다.
		m_node = ResourceManager::Instance().GetNode(fileName);
		LoadAnimation(ResourceManager::Instance().GetAnimations(fileName));
	}

	void SkinningMeshObject::LoadDiffuseMap(const std::string& fileName)
	{
		ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance().GetTexture(fileName);
		m_material->SetAlbedoMap(diffuseTex);
	}

	void SkinningMeshObject::LoadNormalMap(const std::string& fileName)
	{
		ID3D11ShaderResourceView* normalTex = ResourceManager::Instance().GetTexture(fileName);
		m_material->SetNormalMap(normalTex);
	}

	void SkinningMeshObject::LoadARMMap(const std::string& fileName)
	{
        ID3D11ShaderResourceView* armTex = ResourceManager::Instance().GetTexture(fileName);
        m_material->SetOcclusionRoughnessMetalMap(armTex);
    }

	DirectX::XMMATRIX SkinningMeshObject::GetWorldTM()
	{
		if (m_node != nullptr)
			return m_node->rootNodeInvTransform * m_world;

		return m_world;
	}

	bool SkinningMeshObject::IsAnimationEnd()
	{
		return m_currentAnimation->isEnd;
	}

	void SkinningMeshObject::SetOutlineActive(bool isActive)
	{
		if (isActive)
		{
			OutlinePass::outlineObjects.push_back(this);
			m_isOutlineActive = true;
		}
		else
		{
			std::erase_if(OutlinePass::outlineObjects, [&](SkinningMeshObject* obj) { return obj == this; });
			m_isOutlineActive = false;
		}
	}

	void SkinningMeshObject::LoadAnimation(const std::unordered_map<std::string, Animation*>& animation)
	{
		for (auto& e : animation)
		{
			std::string animName = e.first;
			Animation newAnim = *(e.second);
			m_animations.insert(std::make_pair(animName, newAnim));
		}
	}

}
