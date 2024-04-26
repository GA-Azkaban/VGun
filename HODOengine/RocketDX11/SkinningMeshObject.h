#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <wrl.h>
#include "Animation.h"

#include "../HODO3DGraphicsInterface/ISkinnedMesh.h"
#include "../HODO3DGraphicsInterface/Node.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"

#include "MathHeader.h"

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

		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override { m_isActive = isActive; };

		virtual void LoadMesh(const std::string& fileName) override;
		virtual void LoadNode(const std::string& fileName) override;
		virtual void LoadMaterial(HDEngine::IMaterial* material, unsigned int element = 0) override;
		virtual void LoadAlbedoMap(const std::string& fileName, unsigned int element = 0) override;
		virtual void LoadNormalMap(const std::string& fileName, unsigned int element = 0) override;
		virtual void LoadARMMap(const std::string& fileName, unsigned int element = 0) override;
		virtual void LoadRoughnessMap(const std::string& fileName, unsigned int element = 0) override;
		virtual void LoadMetallicMap(const std::string& fileName, unsigned int element = 0) override;
		virtual void SetRoughnessValue(float value, unsigned int element = 0) override;
		virtual void SetMetallicValue(float value, unsigned int element = 0) override;
		virtual void SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element = 0) override;
		virtual Node* GetNode() override;

		virtual void PlayAnimation(const std::string& animName, bool isLoop = true, bool hasExitTime = true, float exitTime = 0.0f) override;
		virtual void PlayAnimationUpper(const std::string& animName, bool isLoop = true, bool hasExitTime = true, float exitTime = 0.0f) override;
		virtual void PlayAnimationLower(const std::string& animName, bool isLoop = true, bool hasExitTime = true, float exitTime = 0.0f) override;
		
		virtual void SetBlendDuration(float duration) override;
		virtual void SetBlendDurationUpper(float duration) override;
		virtual void SetBlendDurationLower(float duration) override;

		virtual bool IsAnimationExitTimeElapsed() override;
		virtual bool IsUpperAnimationExitTimeElapsed() override;
		virtual bool IsLowerAnimationExitTimeElapsed() override;

		virtual bool IsAnimationEnd() override;
		virtual bool IsUpperAnimationEnd() override;
		virtual bool IsLowerAnimationEnd() override;

		virtual void SetOutlineActive(bool isActive) override;

		std::vector<Mesh*>& GetMeshes() { return m_meshes; }
		std::vector<Material*>& GetMaterials() { return m_materials; }
		DirectX::XMMATRIX GetWorldTM();
		std::vector<DirectX::XMMATRIX>& GetBoneTransforms() { return m_boneTransform; }
		bool IsActive() { return m_isActive; }
		bool IsOutlineActive() { return m_isOutlineActive; };
		void SetRenderState(ID3D11RasterizerState* rasterizerState) { m_rasterizerState = rasterizerState; }

	private:
		void LoadAnimation(const std::unordered_map<std::string, Animation*>& animation);

		void UpdateUpperAnimation(float deltaTime);
		void UpdateLowerAnimation(float deltaTime);

		void UpdateAnimation(float animationTime, Node* node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform);
		void UpdateAnimationSeparated(float animationTime, Node* node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform, UINT index);
		DirectX::XMFLOAT3 CalcInterpolatedPosition(float animationTime, NodeAnimation* nodeAnim);
		DirectX::XMFLOAT4 CalcInterpolatedRotation(float animationTime, NodeAnimation* nodeAnim);
		DirectX::XMFLOAT3 CalcInterpolatedScaling(float animationTime, NodeAnimation* nodeAnim);

		void UpdateBlendAnimation(float prevAnimationTime, float animationTime, Node* node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform);
		void UpdateBlendAnimationSeparated(float prevAnimationTime, float animationTime, Node* node, DirectX::XMMATRIX parentTransform, DirectX::XMMATRIX globalInvTransform, UINT index);
		DirectX::XMFLOAT3 CalcBlendedPosition(float prevAnimationTime, float currAnimationTime, float blendDuration, NodeAnimation* prevAnim, NodeAnimation* currentAnim);
		DirectX::XMFLOAT4 CalcBlendedRotation(float prevAnimationTime, float currAnimationTime, float blendDuration, NodeAnimation* prevAnim, NodeAnimation* currentAnim);
		DirectX::XMFLOAT3 CalcBlendedScaling(float prevAnimationTime, float currAnimationTime, float blendDuration, NodeAnimation* prevAnim, NodeAnimation* currentAnim);

	private:
		std::vector<Mesh*> m_meshes;
		std::vector<Material*> m_materials;
		bool m_isActive;
		bool m_receiveTMInfoFlag;
		bool m_isOutlineActive;
		bool m_separateUpperAndLowerAnim;
		bool m_blendFlag;
		bool m_blendFlagUpper;
		bool m_blendFlagLower;
		float m_blendDuration;
		float m_blendDurationUpper;
		float m_blendDurationLower;
		bool m_hasExitTime;
		bool m_hasExitTimeUpper;
		bool m_hasExitTimeLower;
		float m_exitTime;
		float m_exitTimeUpper;
		float m_exitTimeLower;
		bool m_isExitTimeElapsed;
		bool m_isExitTimeUpperElapsed;
		bool m_isExitTimeLowerElapsed;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

		// Animation
		std::unordered_map<std::string, Animation*> m_animations;
		Animation m_savedPreviousAnimation;
		Animation m_savedCurrentAnimation;
		Animation* m_previousAnimation;
		Animation* m_currentAnimation;
		std::vector<DirectX::XMMATRIX> m_boneTransform;
		Node m_node;

		// upper, lower animation
		Animation m_savedUpperPreviousAnimation;
		Animation m_savedLowerPreviousAnimation;
		Animation m_savedUpperCurrentAnimation;
		Animation m_savedLowerCurrentAnimation;
		Animation* m_previousUpperAnimation;
		Animation* m_previousLowerAnimation;
		Animation* m_currentUpperAnimation;
		Animation* m_currentLowerAnimation;

		// Transform Matrix
		DirectX::XMMATRIX m_world;	// Define transformations from local spaces to world space.

		std::unordered_map<std::string, DirectX::XMMATRIX> m_nodeTransformMap;

		VertexShader* m_vertexShader;
		PixelShader* m_pixelShader;

		// 우리 게임에 사용하는 캐릭터 전용 노드 하드 코딩.
		std::unordered_set<std::string> _upperAnimationNodes;
		std::unordered_set<std::string> _lowerAnimationNodes;
		std::unordered_set<std::string> _lowerAnimationNames;
	};
}
