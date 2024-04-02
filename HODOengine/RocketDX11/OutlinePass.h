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
		OutlinePass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer, QuadBuffer* stencilEnableBuffer);
		~OutlinePass();

		static std::vector<SkinningMeshObject*> outlineObjects;

		void Render();

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _stencilColorState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _sobelDetectionState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _stencilDisableState;
		Microsoft::WRL::ComPtr<ID3D11BlendState> _outlineBlendState;
		
		DeferredBuffers* _deferredBuffers;
		QuadBuffer* _stencilEnableBuffer;
		QuadBuffer* _quadBuffer;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		// 아웃라인을 표시할 대상을 스텐실버퍼에 그릴 때 쓰는 컬러값인데,
		// 우리 게임에서 적군은 최대 5명이므로 일단 5개를 잡는다.
		DirectX::XMFLOAT4 stencilColors[5];
	};
}
