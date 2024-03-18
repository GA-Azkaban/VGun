#pragma once
#include <d3d11_2.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class QuadBuffer;
	class StaticMeshObject;
	class SkinningMeshObject;
	class VertexShader;
	class PixelShader;

	class OutlinePass
	{
	public:
		OutlinePass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer);
		~OutlinePass();

		static std::vector<StaticMeshObject*> staticMeshOutlines;
		static std::vector<SkinningMeshObject*> skinningMeshOutlines;

		void Render();

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _depthEnableState;
		DeferredBuffers* _deferredBuffers;
		QuadBuffer* _quadBuffer;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		DirectX::XMMATRIX _lineScale;
	};
}
