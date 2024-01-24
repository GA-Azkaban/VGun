#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "../HODO3DGraphicsInterface/IDebugObject.h"

/// <summary>
/// 3D공간에서 축이나 그리드 등의 형태로 표현되어
/// 위치나 공간 크기 등을 보다 쉽게 볼 수 있도록 하는 오브젝트
/// 
/// 2023.07.06 MJKIM
/// </summary>
 
using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;

	class HelperObject : public HDEngine::IDebugObject
	{
	public:
		HelperObject();
		~HelperObject();

	public:
		void Update(float deltaTime);
		void Render();

		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override { m_isActive = isActive; };

		virtual void SetFillModeSolid() override;
		virtual void SetFillModeWireframe() override;
		virtual void SetColor(const Vector4& color) override;
		virtual void SetMesh(const std::string& meshName) override;

	private:
		ComPtr<ID3D11DeviceContext> m_deviceContext;
		ComPtr<ID3D11RasterizerState> m_RS;
		std::vector<Mesh*> m_meshes;
		Material* m_material;
		bool m_isActive;

		DirectX::XMMATRIX m_world;
	};
}