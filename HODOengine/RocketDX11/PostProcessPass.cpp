#include "PostProcessPass.h"
#include "QuadBuffer.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <d3d11_2.h>

namespace RocketCore::Graphics
{

	PostProcessPass::PostProcessPass(QuadBuffer* quadBuffer)
		: _quadBuffer(quadBuffer)
	{
		_vertexShader = ResourceManager::Instance().GetVertexShader("FullScreenQuadVS.cso");
		_pixelShader = ResourceManager::Instance().GetPixelShader("PostProcessPS.cso");
	}

	PostProcessPass::~PostProcessPass()
	{
		delete _quadBuffer;
		delete _vertexShader;
		delete _pixelShader;
	}

	void PostProcessPass::Render()
	{
		_quadBuffer->SetRenderTargets();
		_quadBuffer->ClearRenderTargets();

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		_vertexShader->SetShader();

		_pixelShader->SetShaderResourceView("src", _quadBuffer->GetShaderResourceView());
		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		ResourceManager::Instance().GetDeviceContext()->Draw(4, 0);

		_quadBuffer->FlushShaderResourceViews();
	}

}
