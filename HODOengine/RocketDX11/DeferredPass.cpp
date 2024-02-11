#include "DeferredPass.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "Light.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	DeferredPass::DeferredPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer)
		: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer)
	{
		_vertexShader = ResourceManager::Instance().GetVertexShader("FullScreenQuadVS.cso");
		_pixelShader = ResourceManager::Instance().GetPixelShader("FullScreenQuadPS.cso");
	}

	void DeferredPass::Render()
	{
		_quadBuffer->SetRenderTargets();
		_quadBuffer->ClearRenderTargets();

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID));

		_vertexShader->SetShader();

		//XMMATRIX invView = XMMatrixInverse(0, Camera::GetMainCamera()->GetViewMatrix());
		//_pixelShader->SetMatrix4x4Array("inverseView", &invView);
		//
		//XMMATRIX invProj = XMMatrixInverse(0, Camera::GetMainCamera()->GetProjectionMatrix());
		//_pixelShader->SetMatrix4x4Array("inverseProjection", &invProj);

		//_pixelShader->SetShaderResourceView("DepthTexture", _deferredBuffers->GetDepthSRV());
		_pixelShader->SetShaderResourceView("Position", _deferredBuffers->GetShaderResourceView(0));
		_pixelShader->SetShaderResourceView("Diffuse", _deferredBuffers->GetShaderResourceView(1));
		_pixelShader->SetShaderResourceView("Normal", _deferredBuffers->GetShaderResourceView(2));
		_pixelShader->SetShaderResourceView("MetalRough", _deferredBuffers->GetShaderResourceView(3));

		if (_deferredBuffers->GetEnvMap())
		{
			_pixelShader->SetShaderResourceView("EnvMap", _deferredBuffers->GetEnvMap());
			_pixelShader->SetShaderResourceView("PrefilteredSpecMap", _deferredBuffers->GetEnvPreFilterMap());
			_pixelShader->SetShaderResourceView("BrdfLUT", _deferredBuffers->GetBRDFLut());
			_pixelShader->SetInt("useEnvMap", 1);
		}
		else
		{
			_pixelShader->SetInt("useEnvMap", 0);
		}

		LightProperties& lp = LightManager::Instance().GetLightProperties();
		XMFLOAT3 cameraPos = Camera::GetMainCamera()->GetPosition();
		lp.cameraPosition = XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);

		_pixelShader->SetFloat4("cameraPosition", lp.cameraPosition);
		_pixelShader->SetFloat4("globalAmbient", lp.globalAmbient);
		_pixelShader->SetLights("lights", &lp.lights[0]);

		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		ResourceManager::Instance().GetDeviceContext()->Draw(4, 0);

		_deferredBuffers->FlushShaderResourceViews();
	}

}
