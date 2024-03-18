#include "DeferredPass.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "Light.h"
#include "ShadowMapPass.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	DeferredPass::DeferredPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer, ShadowMapPass* shadowMapPass)
		: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer), _shadowMapPass(shadowMapPass)
	{
		_vertexShader = ResourceManager::Instance().GetVertexShader("FullScreenQuadVS.cso");
		_pixelShader = ResourceManager::Instance().GetPixelShader("FullScreenQuadPS.cso");
	}

	DeferredPass::~DeferredPass()
	{
		delete _deferredBuffers;
		delete _quadBuffer;
		delete _vertexShader;
		delete _pixelShader;
	}

	void DeferredPass::Render()
	{
		_quadBuffer->SetRenderTargets();
		_quadBuffer->ClearRenderTargets();

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID));

		_vertexShader->SetShader();

		XMMATRIX invView = XMMatrixInverse(0, Camera::GetMainCamera()->GetViewMatrix());
		_pixelShader->SetMatrix4x4Array("inverseView", &invView);
		
		XMMATRIX invProj = XMMatrixInverse(0, Camera::GetMainCamera()->GetProjectionMatrix());
		_pixelShader->SetMatrix4x4Array("inverseProjection", &invProj);

		//_pixelShader->SetShaderResourceView("DepthTexture", _deferredBuffers->GetDepthSRV());
		_pixelShader->SetShaderResourceView("Position", _deferredBuffers->GetShaderResourceView(0));
		_pixelShader->SetShaderResourceView("Diffuse", _deferredBuffers->GetShaderResourceView(1));
		_pixelShader->SetShaderResourceView("Normal", _deferredBuffers->GetShaderResourceView(2));
		_pixelShader->SetShaderResourceView("MetalRough", _deferredBuffers->GetShaderResourceView(3));
		_pixelShader->SetShaderResourceView("AO", _deferredBuffers->GetSSAOMap());
		_pixelShader->SetShaderResourceView("ShadowMap", _deferredBuffers->GetShadowMap());

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
		XMFLOAT3 cameraPos = Camera::GetMainCamera()->GetPosition();
		_pixelShader->SetFloat4("cameraPosition", XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f));
		_pixelShader->SetFloat4("globalAmbient", LightManager::Instance().GetGlobalAmbient());
		_pixelShader->SetLights("lights", &LightManager::Instance().GetLight(0));

		_pixelShader->SetFloat("screenWidth", static_cast<float>(_deferredBuffers->GetScreenWidth()));
		_pixelShader->SetFloat("screenHeight", static_cast<float>(_deferredBuffers->GetScreenHeight()));
		
		_pixelShader->SetMatrix4x4("lightViewProjection", XMMatrixTranspose(LightManager::Instance().GetLightViewProj()));

		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		ResourceManager::Instance().GetDeviceContext()->Draw(4, 0);

		_deferredBuffers->FlushShaderResourceViews();
	}

}
