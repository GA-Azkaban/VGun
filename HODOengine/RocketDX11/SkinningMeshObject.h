#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <unordered_map>
#include <string>
#include "Animation.h"

#include "..\\HODO3DGraphicsInterface\\ISkinnedMesh.h"
#include "..\\HODOmath\HODOmath.h"

/// <summary>
/// 2023.07.07 MJKIM
/// </summary>

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;
	class VertexShader;
	class PixelShader;

	class SkinningMeshObject : public HDEngine::ISkinnedMesh
	{
	public:
		SkinningMeshObject();
		~SkinningMeshObject();

	public:
		void Update(float deltaTime);
		void Render();

		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& worldTM) override;
		virtual void SetActive(bool isActive) override { m_isActive = isActive; };

		virtual void LoadMesh(const std::string& fileName) override;
		virtual void LoadNormalMap(const std::string& fileName);
		virtual void LoadDiffuseMap(const std::string& fileName);
		virtual void LoadVertexShader(const std::string& fileName) override;
		virtual void LoadPixelShader(const std::string& fileName) override;

		virtual void PlayAnimation(const std::string& animName, bool isLoop = true) override;
		virtual void PlayAnimation(UINT index, bool isLoop = true) override;

		void SetSamplerState(ID3D11SamplerState* sampler);

	private:
		void UpdateAnimation(float deltaTime, const Node& node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform);
		DirectX::XMFLOAT3 CalcInterpolatedPosition(float animationTime, NodeAnimation* nodeAnim);
		DirectX::XMFLOAT4 CalcInterpolatedRotation(float animationTime, NodeAnimation* nodeAnim);
		DirectX::XMFLOAT3 CalcInterpolatedScaling(float animationTime, NodeAnimation* nodeAnim);

	private:
		std::vector<Mesh*> m_meshes;
		Material* m_material;
		bool m_isActive;

		// Animation
		std::unordered_map<std::string, Animation*> m_animations;
		Animation* m_currentAnimation;
		std::vector<DirectX::XMMATRIX> m_boneTransform;
		Node* m_node;

		// Transform Matrix
		DirectX::XMMATRIX m_world;	// Define transformations from local spaces to world space.
	};
}