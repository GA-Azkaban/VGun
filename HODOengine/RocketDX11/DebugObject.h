#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include "Animation.h"
#include "..\\HODO3DGraphicsInterface\\IDebugObject.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;

	class DebugObject : public HDEngine::IDebugObject
	{
	public:
		DebugObject();
		~DebugObject();

	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void SetFillModeSolid() override;
		virtual void SetFillModeWireframe() override;
		virtual void SetColor(const Vector4& color) override;
		virtual void LoadMesh(const std::string& fileName) override;

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
		DirectX::XMFLOAT4 m_color;
		Node* m_node;
		Material* m_material;
		bool m_isActive;
	};
}
