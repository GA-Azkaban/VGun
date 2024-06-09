#pragma once
#include "../HODO3DGraphicsInterface/IDecal.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"
#include <d3d11.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;
	class VertexShader;
	class PixelShader;

	class DecalObject : public HDEngine::IDecal
	{
	public:
		DecalObject();
		~DecalObject();
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void LoadMaterial(HDEngine::IMaterial* material) override;
		virtual void SetColor(UINT r, UINT g, UINT b, UINT a) override;
	
	public:
		void Render();
		DirectX::BoundingBox GetBoundingBox();
		bool IsCameraVisible();
		void SetCameraVisible(bool isVisible);

	private:
		ComPtr<ID3D11RasterizerState> _rasterizerState;
		DirectX::XMMATRIX _world;
		Mesh* _mesh;
		Material* _material;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
		DirectX::BoundingBox _boundingBox;
		bool _cameraVisible; // camera culling
		bool _isActive;
		bool _receiveTMInfoFlag;
	};
}
