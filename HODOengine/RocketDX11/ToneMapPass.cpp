#include "ToneMapPass.h"
#include "QuadBuffer.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <d3d11_2.h>

namespace RocketCore::Graphics
{

	ToneMapPass::ToneMapPass(QuadBuffer* quadBuffer, QuadBuffer* toneMapBuffer)
		: _quadBuffer(quadBuffer), _toneMapBuffer(toneMapBuffer)
	{
		_vertexShader = ResourceManager::Instance().GetVertexShader("FullScreenQuadVS.cso");
		//_pixelShader = ResourceManager::Instance().GetPixelShader("ToneMapReinhardPS.cso");
		_pixelShader = ResourceManager::Instance().GetPixelShader("ToneMapAcesPS.cso");
	}

	ToneMapPass::~ToneMapPass()
	{
		delete _quadBuffer;
		delete _toneMapBuffer;
		delete _vertexShader;
		delete _pixelShader;
	}

	void ToneMapPass::Render()
	{
		_toneMapBuffer->SetRenderTargets();
		_toneMapBuffer->ClearRenderTargets();

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		_vertexShader->SetShader();

		_pixelShader->SetShaderResourceView("src", _quadBuffer->GetShaderResourceView());
		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		ResourceManager::Instance().GetDeviceContext()->Draw(4, 0);

		_quadBuffer->FlushShaderResourceViews();
	}

}
