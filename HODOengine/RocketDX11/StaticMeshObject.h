#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include "Animation.h"
#include "..\\HODO3DGraphicsInterface\\IStaticMesh.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;

	class StaticMeshObject : public HDEngine::IStaticMesh
	{
	public:
		StaticMeshObject();
		~StaticMeshObject();

	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void LoadMesh(const std::string& fileName) override;
		virtual void LoadNormalMap(const std::string& fileName) override;
		virtual void LoadDiffuseMap(const std::string& fileName) override;
		virtual void LoadARMMap(const std::string& fileName) override;
		virtual void LoadRoughnessMap(const std::string& fileName) override;
		virtual void LoadMetallicMap(const std::string& fileName) override;
		virtual void SetRoughnessValue(float value) override;
		virtual void SetMetallicValue(float value) override;

	public:
		void Render();

	public:		
		std::vector<Mesh*>& GetMeshes() { return m_meshes; }
		DirectX::XMMATRIX GetWorldTM();
		bool IsActive() { return m_isActive; }
		void SetRenderState(ID3D11RasterizerState* rasterizerState) { m_rasterizerState = rasterizerState; }

	private:
		ComPtr<ID3D11RasterizerState> m_rasterizerState;
		DirectX::XMMATRIX m_world;

		std::vector<Mesh*> m_meshes;
		Node* m_node;
		Material* m_material;
		bool m_isActive;
		bool m_receiveTMInfoFlag;
	};
}
