#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <wrl.h>
#include "Animation.h"

#include "..\\HODO3DGraphicsInterface\\ISkinnedMesh.h"
#include "MathHeader.h"

/// <summary>
/// 2023.07.07 MJKIM
/// </summary>

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;

	class SkinningMeshObject : public HDEngine::ISkinnedMesh
	{
	public:
		SkinningMeshObject();
		~SkinningMeshObject();

	public:
		void Update(float deltaTime);
		void Render();

		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override { m_isActive = isActive; };

		virtual void LoadMesh(const std::string& fileName) override;
		virtual void LoadNormalMap(const std::string& fileName);
		virtual void LoadDiffuseMap(const std::string& fileName);

		virtual void PlayAnimation(const std::string& fileName, bool isLoop = true) override;
		void PlayAnimation(UINT index, bool isLoop = true);

		bool IsAnimationEnd();
	
		std::vector<Mesh*>& GetMeshes() { return m_meshes; }
		DirectX::XMMATRIX GetWorldTM();
		std::vector<DirectX::XMMATRIX>& GetBoneTransform() { return m_boneTransform; }
		bool IsActive() { return m_isActive; }
		void SetRenderState(ID3D11RasterizerState* rasterizerState) { m_rasterizerState = rasterizerState; }

	private:
		void UpdateAnimation(float deltaTime, const Node& node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform);
		DirectX::XMFLOAT3 CalcInterpolatedPosition(float animationTime, NodeAnimation* nodeAnim);
		DirectX::XMFLOAT4 CalcInterpolatedRotation(float animationTime, NodeAnimation* nodeAnim);
		DirectX::XMFLOAT3 CalcInterpolatedScaling(float animationTime, NodeAnimation* nodeAnim);

	private:
		std::vector<Mesh*> m_meshes;
		Material* m_material;
		bool m_isActive;
		bool m_receiveTMInfoFlag;
		bool m_blendFlag;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

		// Animation
		std::unordered_map<std::string, Animation*> m_animations;
		Animation* m_previousAnimation;
		Animation* m_currentAnimation;
		std::vector<DirectX::XMMATRIX> m_boneTransform;
		Node* m_node;

		// Transform Matrix
		DirectX::XMMATRIX m_world;	// Define transformations from local spaces to world space.
	};
}
