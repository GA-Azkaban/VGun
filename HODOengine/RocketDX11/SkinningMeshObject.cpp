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
#include "../HODOCommon/HDTransform.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	SkinningMeshObject::SkinningMeshObject()
		: m_isActive(true), m_receiveTMInfoFlag(false),
		m_world{ XMMatrixIdentity() }, m_separateUpperAndLowerAnim(false), m_isOutlineActive(false),
		m_currentAnimation(nullptr), m_currentUpperAnimation(nullptr), m_currentLowerAnimation(nullptr),
		m_previousAnimation(nullptr), m_previousUpperAnimation(nullptr), m_previousLowerAnimation(nullptr),
		m_blendFlag(false), m_blendFlagUpper(false), m_blendFlagLower(false),
		m_blendDuration(0.1f), m_blendDurationUpper(0.1f), m_blendDurationLower(0.1f),
		m_hasExitTime(false), m_hasExitTimeUpper(false), m_hasExitTimeLower(false),
		m_exitTime(0.0f), m_exitTimeUpper(0.0f), m_exitTimeLower(0.0f),
		m_isExitTimeElapsed(true), m_isExitTimeUpperElapsed(true), m_isExitTimeLowerElapsed(true)
	{
		m_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
		m_boneTransform.resize(96, XMMatrixIdentity());
		m_vertexShader = ResourceManager::Instance().GetVertexShader("SkeletonVertexShader.cso");
		m_pixelShader = ResourceManager::Instance().GetPixelShader("SkeletonPixelShader.cso");

		_upperAnimationNodes.insert("Armature");
		_upperAnimationNodes.insert("root");
		_upperAnimationNodes.insert("ik_hand_root");
		_upperAnimationNodes.insert("ik_hand_gun");
		_upperAnimationNodes.insert("ik_hand_l");
		_upperAnimationNodes.insert("ik_hand_r");
		_upperAnimationNodes.insert("pelvis");
		_upperAnimationNodes.insert("spine_01");
		_upperAnimationNodes.insert("spine_02");
		_upperAnimationNodes.insert("spine_03");
		_upperAnimationNodes.insert("clavicle_l");
		_upperAnimationNodes.insert("upperarm_l");
		_upperAnimationNodes.insert("lowerarm_l");
		_upperAnimationNodes.insert("hand_l");
		_upperAnimationNodes.insert("index_01_l");
		_upperAnimationNodes.insert("index_02_l");
		_upperAnimationNodes.insert("index_03_l");
		_upperAnimationNodes.insert("middle_01_l");
		_upperAnimationNodes.insert("middle_02_l");
		_upperAnimationNodes.insert("middle_03_l");
		_upperAnimationNodes.insert("pinky_01_l");
		_upperAnimationNodes.insert("pinky_02_l");
		_upperAnimationNodes.insert("pinky_03_l");
		_upperAnimationNodes.insert("ring_01_l");
		_upperAnimationNodes.insert("ring_02_l");
		_upperAnimationNodes.insert("ring_03_l");
		_upperAnimationNodes.insert("thumb_01_l");
		_upperAnimationNodes.insert("thumb_02_l");
		_upperAnimationNodes.insert("thumb_03_l");
		_upperAnimationNodes.insert("lowerarm_twist_01_l");
		_upperAnimationNodes.insert("upperarm_twist_01_l");
		_upperAnimationNodes.insert("clavicle_r");
		_upperAnimationNodes.insert("upperarm_r");
		_upperAnimationNodes.insert("lowerarm_r");
		_upperAnimationNodes.insert("hand_r");
		_upperAnimationNodes.insert("index_01_r");
		_upperAnimationNodes.insert("index_02_r");
		_upperAnimationNodes.insert("index_03_r");
		_upperAnimationNodes.insert("middle_01_r");
		_upperAnimationNodes.insert("middle_02_r");
		_upperAnimationNodes.insert("middle_03_r");
		_upperAnimationNodes.insert("pinky_01_r");
		_upperAnimationNodes.insert("pinky_02_r");
		_upperAnimationNodes.insert("pinky_03_r");
		_upperAnimationNodes.insert("ring_01_r");
		_upperAnimationNodes.insert("ring_02_r");
		_upperAnimationNodes.insert("ring_03_r");
		_upperAnimationNodes.insert("thumb_01_r");
		_upperAnimationNodes.insert("thumb_02_r");
		_upperAnimationNodes.insert("thumb_03_r");
		_upperAnimationNodes.insert("lowerarm_twist_01_r");
		_upperAnimationNodes.insert("upperarm_twist_01_r");
		_upperAnimationNodes.insert("neck_01");
		_upperAnimationNodes.insert("head");

		_lowerAnimationNodes.insert("root");
		_lowerAnimationNodes.insert("ik_foot_root");
		_lowerAnimationNodes.insert("ik_foot_l");
		_lowerAnimationNodes.insert("ik_foot_r");
		_lowerAnimationNodes.insert("pelvis");
		_lowerAnimationNodes.insert("thigh_l");
		_lowerAnimationNodes.insert("calf_l");
		_lowerAnimationNodes.insert("calf_twist_01_l");
		_lowerAnimationNodes.insert("foot_l");
		_lowerAnimationNodes.insert("ball_l");
		_lowerAnimationNodes.insert("thigh_twist_01_l");
		_lowerAnimationNodes.insert("thigh_r");
		_lowerAnimationNodes.insert("calf_r");
		_lowerAnimationNodes.insert("calf_twist_01_r");
		_lowerAnimationNodes.insert("foot_r");
		_lowerAnimationNodes.insert("ball_r");
		_lowerAnimationNodes.insert("thigh_twist_01_r");

		_lowerAnimationNames.insert("X_crouch");
		_lowerAnimationNames.insert("X_crouchDown");
		_lowerAnimationNames.insert("X_crouchUp");
		_lowerAnimationNames.insert("X_crouchWalk_B");
		_lowerAnimationNames.insert("X_crouchWalk_F");
		_lowerAnimationNames.insert("AR_crouch");
		_lowerAnimationNames.insert("AR_crouchDown");
		_lowerAnimationNames.insert("AR_crouchUp");
		_lowerAnimationNames.insert("AR_crouchWalk_B");
		_lowerAnimationNames.insert("AR_crouchWalk_F");
	}

	SkinningMeshObject::~SkinningMeshObject()
	{

	}

	void SkinningMeshObject::Update(float deltaTime)
	{
		if (m_separateUpperAndLowerAnim)
		{
			// upper animation
			UpdateUpperAnimation(deltaTime);
			//lower animation
			UpdateLowerAnimation(deltaTime);
		}
		else
		{
			if (m_currentAnimation != nullptr)
			{
				if (m_blendFlag)
				{
					m_currentAnimation->accumulatedTime += deltaTime * m_currentAnimation->ticksPerSecond;

					if (m_currentAnimation->accumulatedTime > (m_blendDuration * m_currentAnimation->ticksPerSecond))
					{
						m_blendFlag = false;
						m_currentAnimation->accumulatedTime = 0.0f;
						return;
					}

					UpdateBlendAnimation(m_previousAnimation->accumulatedTime, m_currentAnimation->accumulatedTime, &m_node, m_world, DirectX::XMMatrixInverse(nullptr, m_world));
				}
				else
				{
					m_currentAnimation->accumulatedTime += deltaTime * m_currentAnimation->ticksPerSecond;

					if (m_currentAnimation->accumulatedTime >= m_exitTime)
					{
						m_isExitTimeElapsed = true;
					}

					if (m_currentAnimation->accumulatedTime > m_currentAnimation->duration)
					{
						m_currentAnimation->isEnd = true;
						if (m_currentAnimation->isLoop == true)
						{
							m_currentAnimation->accumulatedTime = 0.0f;
							m_isExitTimeElapsed = false;
						}
						return;
					}

					if (m_currentAnimation->isEnd == true)
					{
						m_currentAnimation->isEnd = false;
					}

					if (!m_currentAnimation->isEnd)
					{
						UpdateAnimation(m_currentAnimation->accumulatedTime, &m_node, m_world, DirectX::XMMatrixInverse(nullptr, m_world));
					}
				}
			}
		}
	}

	void SkinningMeshObject::UpdateUpperAnimation(float deltaTime)
	{
		if (m_currentUpperAnimation != nullptr)
		{
			if (m_blendFlagUpper)
			{
				m_currentUpperAnimation->accumulatedTime += deltaTime * m_currentUpperAnimation->ticksPerSecond;

				if (m_currentUpperAnimation->accumulatedTime > (m_blendDurationUpper * m_currentUpperAnimation->ticksPerSecond))
				{
					m_blendFlagUpper = false;
					m_currentUpperAnimation->accumulatedTime = 0.0f;
					return;
				}

				UpdateBlendAnimationSeparated(m_previousUpperAnimation->accumulatedTime, m_currentUpperAnimation->accumulatedTime, &m_node, m_world, DirectX::XMMatrixInverse(nullptr, m_world), 0);
			}
			else
			{
				m_currentUpperAnimation->accumulatedTime += deltaTime * m_currentUpperAnimation->ticksPerSecond;

				if (m_currentUpperAnimation->accumulatedTime >= m_exitTimeUpper)
				{
					m_isExitTimeUpperElapsed = true;
				}

				if (m_currentUpperAnimation->accumulatedTime > m_currentUpperAnimation->duration)
				{
					m_currentUpperAnimation->isEnd = true;
					if (m_currentUpperAnimation->isLoop == true)
					{
						m_currentUpperAnimation->accumulatedTime = 0.0f;
						m_isExitTimeUpperElapsed = false;
					}
					return;
				}

				if (m_currentUpperAnimation->isEnd == true)
				{
					m_currentUpperAnimation->isEnd = false;
				}

				if (!m_currentUpperAnimation->isEnd)
				{
					UpdateAnimationSeparated(m_currentUpperAnimation->accumulatedTime, &m_node, m_world, DirectX::XMMatrixInverse(nullptr, m_world), 0);
				}
			}
		}
	}

	void SkinningMeshObject::UpdateLowerAnimation(float deltaTime)
	{
		if (m_currentLowerAnimation != nullptr)
		{
			if (m_blendFlagLower)
			{
				m_currentLowerAnimation->accumulatedTime += deltaTime * m_currentLowerAnimation->ticksPerSecond;

				if (m_currentLowerAnimation->accumulatedTime > (m_blendDurationLower * m_currentLowerAnimation->ticksPerSecond))
				{
					m_blendFlagLower = false;
					m_currentLowerAnimation->accumulatedTime = 0.0f;
					return;
				}

				UpdateBlendAnimationSeparated(m_previousLowerAnimation->accumulatedTime, m_currentLowerAnimation->accumulatedTime, &m_node, m_world, DirectX::XMMatrixInverse(nullptr, m_world), 1);
			}
			else
			{
				m_currentLowerAnimation->accumulatedTime += deltaTime * m_currentLowerAnimation->ticksPerSecond;

				if (m_currentLowerAnimation->accumulatedTime >= m_exitTimeLower)
				{
					m_isExitTimeLowerElapsed = true;
				}

				if (m_currentLowerAnimation->accumulatedTime > m_currentLowerAnimation->duration)
				{
					m_currentLowerAnimation->isEnd = true;
					if (m_currentLowerAnimation->isLoop == true)
					{
						m_currentLowerAnimation->accumulatedTime = 0.0f;
						m_isExitTimeLowerElapsed = false;
					}
					return;
				}

				if (m_currentLowerAnimation->isEnd == true)
				{
					m_currentLowerAnimation->isEnd = false;
				}

				if (!m_currentLowerAnimation->isEnd)
				{
					UpdateAnimationSeparated(m_currentLowerAnimation->accumulatedTime, &m_node, m_world, DirectX::XMMatrixInverse(nullptr, m_world), 1);
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

			XMMATRIX world = m_node.rootNodeInvTransform * m_world;

			m_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(world));
			m_vertexShader->SetMatrix4x4Array("boneTransforms", &m_boneTransform[0], m_boneTransform.size());

			m_vertexShader->CopyAllBufferData();
			m_vertexShader->SetShader();

			for (UINT i = 0; i < m_meshes.size(); ++i)
			{
				if (m_materials[i]->GetAlbedoMap())
				{
					m_pixelShader->SetInt("useAlbedo", 1);
					m_pixelShader->SetShaderResourceView("Albedo", m_materials[i]->GetAlbedoMap());
					m_pixelShader->SetFloat4("albedoColor", m_materials[i]->GetColorFloat4());
				}
				else
				{
					m_pixelShader->SetInt("useAlbedo", 0);
					m_pixelShader->SetFloat4("albedoColor", m_materials[i]->GetColorFloat4());
				}

				if (m_materials[i]->GetNormalMap())
				{
					m_pixelShader->SetInt("useNormalMap", 1);
					m_pixelShader->SetShaderResourceView("NormalMap", m_materials[i]->GetNormalMap());
				}
				else
				{
					m_pixelShader->SetInt("useNormalMap", 0);
				}

				if (m_materials[i]->GetOcclusionRoughnessMetalMap())
				{
					m_pixelShader->SetInt("useOccMetalRough", 1);
					m_pixelShader->SetShaderResourceView("OcclusionRoughnessMetal", m_materials[i]->GetOcclusionRoughnessMetalMap());
				}
				else
				{
					m_pixelShader->SetInt("useOccMetalRough", 0);
					m_pixelShader->SetInt("metallicValue", m_materials[i]->GetMetallicValue());
					m_pixelShader->SetInt("roughnessValue", m_materials[i]->GetRoughnessValue());
				}

				if (m_materials[i]->GetRoughnessMap())
				{
					m_pixelShader->SetInt("useRoughnessMap", 1);
					m_pixelShader->SetShaderResourceView("Roughness", m_materials[i]->GetRoughnessMap());
				}
				else
				{
					m_pixelShader->SetInt("useRoughnessMap", 0);
					m_pixelShader->SetFloat("roughnessValue", m_materials[i]->GetRoughnessValue());
				}

				if (m_materials[i]->GetMetallicMap())
				{
					m_pixelShader->SetInt("useMetallicMap", 1);
					m_pixelShader->SetShaderResourceView("Metallic", m_materials[i]->GetRoughnessMap());
				}
				else
				{
					m_pixelShader->SetInt("useMetallicMap", 0);
					m_pixelShader->SetFloat("metallicValue", m_materials[i]->GetRoughnessValue());
				}

				m_pixelShader->CopyAllBufferData();
				m_pixelShader->SetShader();

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

	void SkinningMeshObject::UpdateAnimation(float animationTime, Node* node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform)
	{
		DirectX::XMMATRIX _nodeTransform = node->nodeTransformOffset;

		NodeAnimation* nodeAnim = nullptr;
		for (UINT i = 0; i < m_currentAnimation->nodeAnimations.size(); ++i)
		{
			if (m_currentAnimation->nodeAnimations[i]->nodeName == node->name)
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
			if (node->nodeTransform != nullptr)
			{
				node->nodeTransform->_position = position;
				node->nodeTransform->_rotation = rotation;
				node->nodeTransform->_scale = scale;
			}
		}
		DirectX::XMMATRIX globalTransform = parentTransform * _nodeTransform;

		m_boneTransform[node->bone.id] = globalInvTransform * globalTransform * node->bone.offset;

		// update values for children bones
		for (Node& child : node->children)
		{
			UpdateAnimation(animationTime, &child, globalTransform, globalInvTransform);
		}
	}

	void SkinningMeshObject::UpdateAnimationSeparated(float animationTime, Node* node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform, UINT index)
	{
		DirectX::XMMATRIX _nodeTransform = node->nodeTransformOffset;
		NodeAnimation* nodeAnim = nullptr;
		if (index == 0)
		{
			for (UINT i = 0; i < m_currentUpperAnimation->nodeAnimations.size(); ++i)
			{
				if (m_currentUpperAnimation->nodeAnimations[i]->nodeName == node->name)
				{
					if (_upperAnimationNodes.find(node->name) != _upperAnimationNodes.end())
					{
						if (node->name == "root" || node->name == "pelvis")
						{
							// crouch 애니메이션이라면 하체의 root 와 pelvis로 갱신
							if (_lowerAnimationNames.find(m_currentLowerAnimation->animName) != _lowerAnimationNames.end())
							{
								nodeAnim = m_currentLowerAnimation->nodeAnimations[i];
								break;
							}
						}
						nodeAnim = m_currentUpperAnimation->nodeAnimations[i];
						break;
					}
				}
			}
		}
		else
		{
			for (UINT i = 0; i < m_currentLowerAnimation->nodeAnimations.size(); ++i)
			{
				if (m_currentLowerAnimation->nodeAnimations[i]->nodeName == node->name)
				{
					if (_lowerAnimationNodes.find(node->name) != _lowerAnimationNodes.end())
					{
						if (node->name == "root" || node->name == "pelvis")
						{
							// crouch 애니메이션이 아니라면 상체의 root 와 pelvis로 갱신
							if (_lowerAnimationNames.find(m_currentLowerAnimation->animName) == _lowerAnimationNames.end())
							{
								nodeAnim = m_currentUpperAnimation->nodeAnimations[i];
								break;
							}
						}
						nodeAnim = m_currentLowerAnimation->nodeAnimations[i];
						break;
					}
				}
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
			if (node->nodeTransform != nullptr)
			{
				node->nodeTransform->_position = position;
				node->nodeTransform->_rotation = rotation;
				node->nodeTransform->_scale = scale;
			}
		}
		DirectX::XMMATRIX globalTransform = parentTransform * _nodeTransform;

		if (index == 0)
		{
			if (_upperAnimationNodes.find(node->name) != _upperAnimationNodes.end())
			{
				m_boneTransform[node->bone.id] = globalInvTransform * globalTransform * node->bone.offset;
			}
		}
		else
		{
			if (_lowerAnimationNodes.find(node->name) != _lowerAnimationNodes.end())
			{
				m_boneTransform[node->bone.id] = globalInvTransform * globalTransform * node->bone.offset;
			}
		}

		// update values for children bones
		for (Node& child : node->children)
		{
			UpdateAnimationSeparated(animationTime, &child, globalTransform, globalInvTransform, index);
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

	void SkinningMeshObject::UpdateBlendAnimation(float prevAnimationTime, float animationTime, Node* node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform)
	{
		DirectX::XMMATRIX _nodeTransform = (node->nodeTransformOffset);

		NodeAnimation* prevAnim = nullptr;
		for (UINT i = 0; i < m_previousAnimation->nodeAnimations.size(); ++i)
		{
			if (m_previousAnimation->nodeAnimations[i]->nodeName == node->name)
			{
				prevAnim = m_previousAnimation->nodeAnimations[i];
				break;
			}
		}

		NodeAnimation* currAnim = nullptr;
		for (UINT i = 0; i < m_currentAnimation->nodeAnimations.size(); ++i)
		{
			if (m_currentAnimation->nodeAnimations[i]->nodeName == node->name)
			{
				currAnim = m_currentAnimation->nodeAnimations[i];
				break;
			}
		}

		float blendDuration = m_blendDuration * m_currentAnimation->ticksPerSecond;

		if (prevAnim != nullptr && currAnim != nullptr)
		{
			// calculate interpolated position
			DirectX::XMFLOAT3 position = CalcBlendedPosition(prevAnimationTime, animationTime, blendDuration, prevAnim, currAnim);
			XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z);

			// calculate interpolated rotation
			DirectX::XMFLOAT4 rotation = CalcBlendedRotation(prevAnimationTime, animationTime, blendDuration, prevAnim, currAnim);
			DirectX::XMVECTOR r = XMLoadFloat4(&rotation);
			DirectX::XMMATRIX rot = XMMatrixRotationQuaternion(r);

			DirectX::XMFLOAT3 scale = CalcBlendedScaling(prevAnimationTime, animationTime, blendDuration, prevAnim, currAnim);
			XMMATRIX sc = XMMatrixScaling(scale.x, scale.y, scale.z);

			_nodeTransform = XMMatrixTranspose(sc * rot * trans);
			if (node->nodeTransform != nullptr)
			{
				node->nodeTransform->_position = position;
				node->nodeTransform->_rotation = rotation;
				node->nodeTransform->_scale = scale;
			}
		}
		DirectX::XMMATRIX globalTransform = parentTransform * _nodeTransform;

		m_boneTransform[node->bone.id] = globalInvTransform * globalTransform * node->bone.offset;

		// update values for children bones
		for (Node& child : node->children)
		{
			UpdateBlendAnimation(prevAnimationTime, animationTime, &child, globalTransform, globalInvTransform);
		}
	}

	void SkinningMeshObject::UpdateBlendAnimationSeparated(float prevAnimationTime, float animationTime, Node* node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform, UINT index)
	{
		DirectX::XMMATRIX _nodeTransform = (node->nodeTransformOffset);
		NodeAnimation* prevAnim = nullptr;
		NodeAnimation* currAnim = nullptr;
		float blendDuration = 0.0f;

		if (index == 0)
		{
			for (UINT i = 0; i < m_previousUpperAnimation->nodeAnimations.size(); ++i)
			{
				if (_upperAnimationNodes.find(node->name) != _upperAnimationNodes.end())
				{
					if (node->name == "root" || node->name == "pelvis")
					{
						//if (m_previousLowerAnimation->nodeAnimations[i]->nodeName == node->name)
						// crouch 애니메이션 이라면
						if (_lowerAnimationNames.find(m_previousLowerAnimation->animName) != _lowerAnimationNames.end())
						{
							prevAnim = m_previousLowerAnimation->nodeAnimations[i];
							break;
						}
					}

					if (m_previousUpperAnimation->nodeAnimations[i]->nodeName == node->name)
					{
						prevAnim = m_previousUpperAnimation->nodeAnimations[i];
						break;
					}
				}
			}

			for (UINT i = 0; i < m_currentUpperAnimation->nodeAnimations.size(); ++i)
			{
				if (m_currentUpperAnimation->nodeAnimations[i]->nodeName == node->name)
				{
					if (_upperAnimationNodes.find(node->name) != _upperAnimationNodes.end())
					{
						if (m_currentUpperAnimation->animName != m_currentLowerAnimation->animName)
						{
							if (node->name == "root" || node->name == "pelvis")
							{
								//if (m_currentLowerAnimation->nodeAnimations[i]->nodeName == node->name)
								for (UINT j = 0; j < m_currentLowerAnimation->nodeAnimations.size(); ++j)
								{
									if (m_currentLowerAnimation->nodeAnimations[j]->nodeName == node->name)
									{
										currAnim = m_currentLowerAnimation->nodeAnimations[j];
										blendDuration = m_blendDurationUpper * m_currentUpperAnimation->ticksPerSecond;
									}
									break;
								}
							}
						}

						currAnim = m_currentUpperAnimation->nodeAnimations[i];
						blendDuration = m_blendDurationUpper * m_currentUpperAnimation->ticksPerSecond;
						break;
					}
				}
			}
		}
		else
		{
			for (UINT i = 0; i < m_previousLowerAnimation->nodeAnimations.size(); ++i)
			{
				if (m_previousLowerAnimation->nodeAnimations[i]->nodeName == node->name)
				{
					if (_lowerAnimationNodes.find(node->name) != _lowerAnimationNodes.end())
					{
						prevAnim = m_previousLowerAnimation->nodeAnimations[i];
						break;
					}
				}
			}

			for (UINT i = 0; i < m_currentLowerAnimation->nodeAnimations.size(); ++i)
			{
				if (m_currentLowerAnimation->nodeAnimations[i]->nodeName == node->name)
				{
					if (_lowerAnimationNodes.find(node->name) != _lowerAnimationNodes.end())
					{
						currAnim = m_currentLowerAnimation->nodeAnimations[i];
						blendDuration = m_blendDurationLower * m_currentLowerAnimation->ticksPerSecond;
						break;
					}
				}
			}
		}

		if (prevAnim != nullptr && currAnim != nullptr)
		{
			// calculate interpolated position
			DirectX::XMFLOAT3 position = CalcBlendedPosition(prevAnimationTime, animationTime, blendDuration, prevAnim, currAnim);
			XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z);

			// calculate interpolated rotation
			DirectX::XMFLOAT4 rotation = CalcBlendedRotation(prevAnimationTime, animationTime, blendDuration, prevAnim, currAnim);
			DirectX::XMVECTOR r = XMLoadFloat4(&rotation);
			DirectX::XMMATRIX rot = XMMatrixRotationQuaternion(r);

			DirectX::XMFLOAT3 scale = CalcBlendedScaling(prevAnimationTime, animationTime, blendDuration, prevAnim, currAnim);
			XMMATRIX sc = XMMatrixScaling(scale.x, scale.y, scale.z);

			_nodeTransform = XMMatrixTranspose(sc * rot * trans);
			if (node->nodeTransform != nullptr)
			{
				node->nodeTransform->_position = position;
				node->nodeTransform->_rotation = rotation;
				node->nodeTransform->_scale = scale;
			}
		}
		DirectX::XMMATRIX globalTransform = parentTransform * _nodeTransform;

		if (index == 0)
		{
			if (_upperAnimationNodes.find(node->name) != _upperAnimationNodes.end())
			{
				m_boneTransform[node->bone.id] = globalInvTransform * globalTransform * node->bone.offset;
			}
		}
		else
		{
			if (_lowerAnimationNodes.find(node->name) != _lowerAnimationNodes.end())
			{
				m_boneTransform[node->bone.id] = globalInvTransform * globalTransform * node->bone.offset;
			}
		}

		// update values for children bones
		for (Node& child : node->children)
		{
			UpdateBlendAnimationSeparated(prevAnimationTime, animationTime, &child, globalTransform, globalInvTransform, index);
		}
	}

	DirectX::XMFLOAT3 SkinningMeshObject::CalcBlendedPosition(float prevAnimationTime, float currAnimationTime, float blendDuration, NodeAnimation* prevAnim, NodeAnimation* currentAnim)
	{
		UINT positionIndex = prevAnim->positionTimestamps.size() - 1;
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
		UINT rotationIndex = prevAnim->rotationTimestamps.size() - 1;
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
		UINT scaleIndex = prevAnim->scaleTimestamps.size() - 1;
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

	void SkinningMeshObject::PlayAnimation(const std::string& animName, bool isLoop /*= true*/, bool hasExitTime /*= true*/, float exitTime /*= 0.0f*/)
	{
		// exitTime을 가지고 있는 경우
		// exitTime만큼 지나지 않았다면 다음 애니메이션으로 넘어갈 수 없다.
		if (m_hasExitTime)
		{
			if (!m_isExitTimeElapsed)
			{
				return;
			}
		}

		// 애니메이션 리스트에 없는 애니메이션은 재생할 수 없다.
		auto animIter = m_animations.find(animName);
		if (animIter == m_animations.end())
		{
			m_currentAnimation = nullptr;
			return;
		}

		// 애니메이션 분리 여부 저장		
		m_separateUpperAndLowerAnim = false;

		// 현재 실행 중인 애니메이션이 있다면
		// 그 애니메이션을 이전 애니메이션으로 저장해두고 다음 애니메이션으로 넘어간다.
		if (m_currentAnimation != nullptr)
		{
			/// 중복 입력 버그가 있어 넣어두는 코드
			// 현재 실행 중인 애니메이션과 실행시키려는 애니메이션 이름이 같다면
			if (m_currentAnimation->animName == animIter->second->animName)
			{
				// 루프 여부 저장
				m_currentAnimation->isLoop = isLoop;
				// 루프가 true인 애니메이션은 실행하던 애니메이션을 바꾸지 않고 종료
				if (m_currentAnimation->isLoop)
				{
					// hasExitTime 여부와 exitTime 저장
					m_hasExitTime = hasExitTime;
					if (hasExitTime)
					{
						m_exitTime = exitTime * m_currentAnimation->ticksPerSecond;
					}
					return;
				}
			}
			// 루프가 false라면 
			// 그리고 실행 중인 애니메이션과 실행시키려는 애니메이션의 이름이 다른 경우
			// exitTime이 있는 경우는 위에서 exitTime 지났는지 체크하고 있다.
			// exitTime이 없는 경우는 애니메이션이 종료되어야 전환 가능
			if (!m_hasExitTime)
			{
				if (!m_currentAnimation->isEnd)
				{
					return;
				}
			}

			// 이전 애니메이션을 저장해둔다.
			m_savedPreviousAnimation = *m_currentAnimation;
			m_previousAnimation = &m_savedPreviousAnimation;
		}
		else
		{
			// 현재 애니메이션이 없다면 이전 애니메이션을 저장할 필요가 없다.
			m_previousAnimation = nullptr;
		}

		// 애니메이션 전환
		m_savedCurrentAnimation = *(animIter->second);
		m_savedUpperCurrentAnimation = *(animIter->second);
		m_savedLowerCurrentAnimation = *(animIter->second);
		m_currentAnimation = &m_savedCurrentAnimation;
		m_currentUpperAnimation = &m_savedUpperCurrentAnimation;
		m_currentLowerAnimation = &m_savedLowerCurrentAnimation;

		// hasExitTime 여부와 exitTime 저장
		m_hasExitTime = hasExitTime;
		if (hasExitTime)
		{
			m_exitTime = exitTime * m_currentAnimation->ticksPerSecond;
		}
		// 루프 여부 저장
		m_currentAnimation->isLoop = isLoop;
		// 애니메이션 실행 시간 초기화
		m_currentAnimation->accumulatedTime = 0.0f;
		m_currentAnimation->isEnd = false;
		m_isExitTimeElapsed = false;

		// 이전 애니메이션이 없다면 블렌딩 하지 않는다.
		if (m_previousAnimation != nullptr)
		{
			m_blendFlag = true;
		}
	}

	void SkinningMeshObject::PlayAnimationUpper(const std::string& animName, bool isLoop /*= true*/, bool hasExitTime /*= true*/, float exitTime /*= 0.0f*/)
	{
		// exitTime을 가지고 있는 경우
		// exitTime만큼 지나지 않았다면 다음 애니메이션으로 넘어갈 수 없다.
		if (m_hasExitTimeUpper)
		{
			if (!m_isExitTimeUpperElapsed)
			{
				return;
			}
		}

		auto animIter = m_animations.find(animName);
		if (animIter == m_animations.end())
		{
			m_currentUpperAnimation = nullptr;
			return;
		}

		// 애니메이션 분리 여부 저장
		m_separateUpperAndLowerAnim = true;

		if (m_currentUpperAnimation != nullptr)
		{
			// 현재 실행하고 있는 애니메이션과 같은 애니메이션 또 재생 눌렸을 때
			if (m_currentUpperAnimation->animName == animIter->second->animName)
			{
				// 루프 여부 저장
				m_currentUpperAnimation->isLoop = isLoop;
				// 루프가 true면 실행되고 있는 애니메이션 전환하지 않고
				// 받아온 정보들만 저장해준다.
				// false면 애니메이션 처음부터 다시 재생될 것이다.
				if (m_currentUpperAnimation->isLoop)
				{
					m_hasExitTimeUpper = hasExitTime;
					if (hasExitTime)
					{
						m_exitTimeUpper = exitTime * m_currentUpperAnimation->ticksPerSecond;
					}
					return;
				}
			}

			// exitTime이 없는 경우에
			// 애니메이션 재생이 모두 끝나지 않았다면 전환하지 않는다.
			if (!m_hasExitTimeUpper)
			{
				if (!m_currentUpperAnimation->isEnd)
				{
					return;
				}
			}

			// 이전 애니메이션 저장한다.
			m_savedUpperPreviousAnimation = *m_currentUpperAnimation;
			m_previousUpperAnimation = &m_savedUpperPreviousAnimation;
		}
		else
		{
			// 현재 애니메이션이 없다면 이전 애니메이션을 저장할 필요가 없다.
			m_previousUpperAnimation = nullptr;
		}

		// currentAnimation은 우선 nullptr로 저장한다. 
		// PlayAnimationUpper에서 PlayAnimation으로 애니메이션 전환을 하는 경우도 생각해봐야 한다.
		m_currentAnimation = nullptr;

		// 애니메이션 전환
		m_savedUpperCurrentAnimation = *(animIter->second);
		m_currentUpperAnimation = &m_savedUpperCurrentAnimation;

		// hasExitTime 여부와 exitTime 저장
		m_hasExitTimeUpper = hasExitTime;
		if (hasExitTime)
		{
			m_exitTimeUpper = exitTime * m_currentUpperAnimation->ticksPerSecond;
		}

		// 루프 여부 저장
		m_currentUpperAnimation->isLoop = isLoop;
		// 애니메이션 실행 시간 초기화
		m_currentUpperAnimation->accumulatedTime = 0.0f;
		m_currentUpperAnimation->isEnd = false;
		m_isExitTimeUpperElapsed = false;

		// 우선 블렌딩 없이
		// 이전 애니메이션이 없다면 블렌딩 하지 않는다.
		//if (m_previousUpperAnimation != nullptr)
		//{
		//	m_blendFlagUpper = true;
		//}
	}

	void SkinningMeshObject::PlayAnimationLower(const std::string& animName, bool isLoop /*= true*/, bool hasExitTime /*= true*/, float exitTime /*= 0.0f*/)
	{
		// exitTime을 가지고 있는 경우
		// exitTime만큼 지나지 않았다면 다음 애니메이션으로 넘어갈 수 없다.
		if (m_hasExitTimeLower)
		{
			if (!m_isExitTimeLowerElapsed)
			{
				return;
			}
		}

		auto animIter = m_animations.find(animName);
		if (animIter == m_animations.end())
		{
			m_currentLowerAnimation = nullptr;
			return;
		}

		// 애니메이션 분리 여부 저장
		m_separateUpperAndLowerAnim = true;

		if (m_currentLowerAnimation != nullptr)
		{
			if (m_currentLowerAnimation->animName == animIter->second->animName)
			{
				m_currentLowerAnimation->isLoop = isLoop;
				if (m_currentLowerAnimation->isLoop)
				{
					m_hasExitTimeLower = hasExitTime;
					if (hasExitTime)
					{
						m_exitTimeLower = exitTime * m_currentLowerAnimation->ticksPerSecond;
					}
					return;
				}
			}

			// exitTime이 없는 경우에
			// 애니메이션 재생이 모두 끝나지 않았다면 전환하지 않는다.
			if (!m_hasExitTimeLower)
			{
				if (!m_currentLowerAnimation->isEnd)
				{
					return;
				}
			}

			m_savedLowerPreviousAnimation = *m_currentLowerAnimation;
			m_previousLowerAnimation = &m_savedLowerPreviousAnimation;
		}
		else
		{
			m_previousLowerAnimation = nullptr;
		}

		m_currentAnimation = nullptr;

		m_savedLowerCurrentAnimation = *(animIter->second);
		m_currentLowerAnimation = &m_savedLowerCurrentAnimation;

		m_hasExitTimeLower = hasExitTime;
		if (hasExitTime)
		{
			m_exitTimeLower = exitTime * m_currentLowerAnimation->ticksPerSecond;
		}

		m_currentLowerAnimation->isLoop = isLoop;
		m_currentLowerAnimation->accumulatedTime = 0.0f;
		m_currentLowerAnimation->isEnd = false;
		m_isExitTimeLowerElapsed = false;
		// 우선 블렌딩 없이
		//if (m_previousLowerAnimation != nullptr)
		//{
		//	m_blendFlagLower = true;
		//}
	}

	void SkinningMeshObject::LoadMesh(const std::string& fileName)
	{
		m_meshes = ResourceManager::Instance().GetMeshes(fileName);
		m_materials = ResourceManager::Instance().GetMaterials(fileName);
	}

	void SkinningMeshObject::LoadNode(const std::string& fileName)
	{
		m_node = *(ResourceManager::Instance().GetNode(fileName));
		LoadAnimation(ResourceManager::Instance().GetAnimations(fileName));
	}

	void SkinningMeshObject::LoadMaterial(HDEngine::IMaterial* material, unsigned int element /*= 0*/)
	{
		if (element >= m_materials.size())
			return;

		Material* newMat = dynamic_cast<Material*>(material);
		if (newMat != nullptr)
		{
			m_materials[element] = newMat;
		}
	}

	void SkinningMeshObject::LoadAlbedoMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;

		if (fileName == "")
			return;

		m_materials[element]->LoadAlbedoTexture(fileName);
	}

	void SkinningMeshObject::LoadNormalMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;

		if (fileName == "")
			return;

		m_materials[element]->LoadNormalTexture(fileName);
	}

	void SkinningMeshObject::LoadARMMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;

		if (fileName == "")
			return;

		m_materials[element]->LoadARMTexture(fileName);
	}

	void SkinningMeshObject::LoadRoughnessMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;

		if (fileName == "")
			return;

		m_materials[element]->LoadRoughnessTexture(fileName);
	}

	void SkinningMeshObject::LoadMetallicMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;

		if (fileName == "")
			return;

		m_materials[element]->LoadMetallicTexture(fileName);
	}

	void SkinningMeshObject::SetRoughnessValue(float value, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;
		m_materials[element]->SetRoughnessValue(value);
	}

	void SkinningMeshObject::SetMetallicValue(float value, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;
		m_materials[element]->SetMetallicValue(value);
	}

	void SkinningMeshObject::SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element /* = 0 */)
	{
		if (element >= m_materials.size())
			return;
		m_materials[element]->SetColor(r, g, b, a);
	}

	DirectX::XMMATRIX SkinningMeshObject::GetWorldTM()
	{
		return m_node.rootNodeInvTransform * m_world;
	}

	void SkinningMeshObject::SetBlendDuration(float duration)
	{
		m_blendDuration = duration;
	}

	void SkinningMeshObject::SetBlendDurationUpper(float duration)
	{
		m_blendDurationUpper = duration;
	}

	void SkinningMeshObject::SetBlendDurationLower(float duration)
	{
		m_blendDurationLower = duration;
	}

	bool SkinningMeshObject::IsAnimationExitTimeElapsed()
	{
		return m_isExitTimeElapsed;
	}

	bool SkinningMeshObject::IsUpperAnimationExitTimeElapsed()
	{
		return m_isExitTimeUpperElapsed;
	}

	bool SkinningMeshObject::IsLowerAnimationExitTimeElapsed()
	{
		return m_isExitTimeLowerElapsed;
	}

	bool SkinningMeshObject::IsAnimationEnd()
	{
		return m_currentAnimation->isEnd;
	}

	bool SkinningMeshObject::IsUpperAnimationEnd()
	{
		return m_currentUpperAnimation->isEnd;
	}

	bool SkinningMeshObject::IsLowerAnimationEnd()
	{
		return m_currentLowerAnimation->isEnd;
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
		m_animations = animation;
	}

	Node* SkinningMeshObject::GetNode()
	{
		return &m_node;
	}

	void SkinningMeshObject::SetFillModeWireFrame(bool setWireFrame)
	{
		if (setWireFrame)
		{
			m_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::WIREFRAME);
		}
		else
		{
			m_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
		}
	}

}
