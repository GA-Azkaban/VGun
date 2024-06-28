#include "PostProcessPass.h"
#include "QuadBuffer.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Material.h"
#include "ObjectManager.h"
#include <d3d11_2.h>

namespace RocketCore::Graphics
{

	PostProcessPass::PostProcessPass(QuadBuffer* quadBuffer)
		: _quadBuffer(quadBuffer)
	{
		_vertexShader = ResourceManager::Instance().GetVertexShader("FullScreenQuadVS.cso");
		_pixelShader = ResourceManager::Instance().GetPixelShader("PostProcessPS.cso");
		HDEngine::MaterialDesc vignetteDesc;
		vignetteDesc.materialName = "vignette";
		vignetteDesc.albedo = "blackout0003.png";
		vignetteDesc.color = { 255, 255, 255, 100 };
		_material = ObjectManager::Instance().CreateMaterial(vignetteDesc);
	}

	PostProcessPass::~PostProcessPass()
	{
		
	}

	void PostProcessPass::Render()
	{
		_quadBuffer->SetRenderTargets();

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		_vertexShader->SetShader();

		_pixelShader->SetInt("vignetteEnabled", 1);
		_pixelShader->SetFloat("vignetteIntensity", 1.0f);
		_pixelShader->SetFloat4("albedoColor", _material->GetColorFloat4());
		_pixelShader->SetShaderResourceView("vignetteSrc", _material->GetAlbedoMap());
		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		ResourceManager::Instance().GetDeviceContext()->Draw(4, 0);

		_quadBuffer->FlushShaderResourceViews();
	}

}
