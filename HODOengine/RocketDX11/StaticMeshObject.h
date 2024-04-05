#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include "Animation.h"
#include "../HODO3DGraphicsInterface/IStaticMesh.h"
#include "../HODO3DGraphicsInterface/Node.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;
	class VertexShader;
	class PixelShader;

	class StaticMeshObject : public HDEngine::IStaticMesh
	{
	public:
		StaticMeshObject();
		~StaticMeshObject();

	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void LoadMesh(const std::string& fileName) override;
		virtual void LoadMaterial(HDEngine::IMaterial* material, unsigned int element = 0) override;
		virtual void LoadAlbedoMap(const std::string& fileName, unsigned int element = 0) override;
		virtual void LoadNormalMap(const std::string& fileName, unsigned int element = 0) override;
		virtual void LoadARMMap(const std::string& fileName, unsigned int element = 0) override;
		virtual void LoadRoughnessMap(const std::string& fileName, unsigned int element = 0) override;
		virtual void LoadMetallicMap(const std::string& fileName, unsigned int element = 0) override;
		virtual void SetRoughnessValue(float value, unsigned int element = 0) override;
		virtual void SetMetallicValue(float value, unsigned int element = 0) override;
		virtual void SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element = 0) override;

	public:
		void Render();

	public:		
		std::vector<Mesh*>& GetMeshes() { return m_meshes; }
		std::vector<Material*>& GetMaterials() { return m_materials; }
		DirectX::XMMATRIX GetWorldTM();
		bool IsActive() { return m_isActive; }
		void SetRenderState(ID3D11RasterizerState* rasterizerState) { m_rasterizerState = rasterizerState; }

	private:
		ComPtr<ID3D11RasterizerState> m_rasterizerState;
		DirectX::XMMATRIX m_world;

		std::vector<Mesh*> m_meshes;
		std::vector<Material*> m_materials;
		Node* m_node;
		bool m_isActive;
		bool m_receiveTMInfoFlag;

		VertexShader* m_vertexShader;
		PixelShader* m_pixelShader;
	};
}
