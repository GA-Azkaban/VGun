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
		virtual void LoadNormalMap(const std::string& fileName) override;
		virtual void LoadAlbedoMap(const std::string& fileName) override;
		virtual void LoadARMMap(const std::string& fileName) override;
		virtual void LoadRoughnessMap(const std::string& fileName) override;
		virtual void LoadMetallicMap(const std::string& fileName) override;
		virtual void SetRoughnessValue(float value) override;
		virtual void SetMetallicValue(float value) override;
		virtual void SetAlbedoColor(UINT r, UINT g, UINT b, UINT a = 255) override;
		virtual void SetAlbedoColor(Vector4 color) override;

		virtual void PlayAnimation(const std::string& fileName, bool isLoop = true) override;
		void PlayAnimation(UINT index, bool isLoop = true);

		virtual bool IsAnimationEnd() override;

		virtual void SetOutlineActive(bool isActive) override;

		virtual const Matrix& GetBoneTransformByNodeName(std::string nodeName) override;

		std::vector<Mesh*>& GetMeshes() { return m_meshes; }
		DirectX::XMMATRIX GetWorldTM();
		std::vector<DirectX::XMMATRIX>& GetBoneTransforms() { return m_boneTransform; }
		bool IsActive() { return m_isActive; }
		bool IsOutlineActive() { return m_isOutlineActive; };
		void SetRenderState(ID3D11RasterizerState* rasterizerState) { m_rasterizerState = rasterizerState; }

	private:
		void LoadAnimation(const std::unordered_map<std::string, Animation*>& animation);

		void UpdateAnimation(float animationTime, const Node& node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform);
		DirectX::XMFLOAT3 CalcInterpolatedPosition(float animationTime, NodeAnimation* nodeAnim);
		DirectX::XMFLOAT4 CalcInterpolatedRotation(float animationTime, NodeAnimation* nodeAnim);
		DirectX::XMFLOAT3 CalcInterpolatedScaling(float animationTime, NodeAnimation* nodeAnim);

		void UpdateBlendAnimation(float prevAnimationTime, float animationTime, const Node& node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform);
		DirectX::XMFLOAT3 CalcBlendedPosition(float prevAnimationTime, float currAnimationTime, float blendDuration, NodeAnimation* prevAnim, NodeAnimation* currentAnim);
		DirectX::XMFLOAT4 CalcBlendedRotation(float prevAnimationTime, float currAnimationTime, float blendDuration, NodeAnimation* prevAnim, NodeAnimation* currentAnim);
		DirectX::XMFLOAT3 CalcBlendedScaling(float prevAnimationTime, float currAnimationTime, float blendDuration, NodeAnimation* prevAnim, NodeAnimation* currentAnim);

	private:
		std::vector<Mesh*> m_meshes;
		Material* m_material;
		bool m_isActive;
		bool m_receiveTMInfoFlag;
		bool m_blendFlag;
		bool m_isOutlineActive;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

		// Animation
		std::unordered_map<std::string, Animation> m_animations;
		Animation* m_previousAnimation;
		Animation* m_currentAnimation;
		std::vector<DirectX::XMMATRIX> m_boneTransform;
		std::unordered_map<std::string, DirectX::XMMATRIX> m_boneTransformMap;
		Node* m_node;

		// Transform Matrix
		DirectX::XMMATRIX m_world;	// Define transformations from local spaces to world space.
	};
}
