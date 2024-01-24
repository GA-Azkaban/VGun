#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#include "Mesh.h"
#include "Model.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "..\\HODO3DGraphicsInterface\\IStaticMesh.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
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
		virtual void LoadVertexShader(const std::string& fileName) override;
		virtual void LoadPixelShader(const std::string& fileName) override;

	public:
		void Render();

	public:
		void SetModel(Model* model);
		void SetVertexShader(VertexShader* shader);
		void SetPixelShader(PixelShader* shader);
		void SetRenderState(ID3D11RasterizerState* renderState) { m_renderState = renderState; }

	private:
		Model* _model;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		ComPtr<ID3D11RasterizerState> m_renderState;
		DirectX::XMMATRIX m_world;

		std::vector<Mesh*> m_meshes;
		Material* m_material;
		bool m_isActive;
	};
}
	