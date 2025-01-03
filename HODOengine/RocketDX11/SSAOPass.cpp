﻿#include "SSAOPass.h"
#include "DeferredBuffers.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"
#include <random>
#include <vector>
using namespace DirectX;

namespace RocketCore::Graphics
{
	SSAOPass::SSAOPass(DeferredBuffers* deferredBuffers)
		: _deferredBuffers(deferredBuffers), _deviceContext(ResourceManager::Instance().GetDeviceContext())
	{
		_vertexShader = ResourceManager::Instance().GetVertexShader("FullScreenQuadVS.cso");
		_pixelShader = ResourceManager::Instance().GetPixelShader("SSAOPixelShader.cso");
		CreateTexture(_deferredBuffers->GetScreenWidth(), _deferredBuffers->GetScreenHeight());
	}

	SSAOPass::~SSAOPass()
	{
		delete _deferredBuffers;
		delete _vertexShader;
		delete _pixelShader;
		_noiseTexture.Reset();
		_noiseSRV.Reset();
	}

	void SSAOPass::Render()
	{
		_deferredBuffers->SetSSAORenderTarget();
		_deferredBuffers->ClearSSAORenderTarget();
		
		_vertexShader->SetShader();
		
		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();

		_pixelShader->SetMatrix4x4("view", XMMatrixTranspose(view));
		_pixelShader->SetMatrix4x4("projection", XMMatrixTranspose(proj));
		_pixelShader->SetMatrix4x4("inverseProjection", XMMatrixInverse(nullptr, proj));

		_pixelShader->SetFloat2("ssaoNoiseScale", XMFLOAT2(_windowSize.x / 4.0f, _windowSize.y / 4.0f));
		_pixelShader->SetFloat4Array("ssaoSamples", &_ssaoSamples[0], 64);
		_pixelShader->SetFloat("ssaoRadius", 1.0f);
		_pixelShader->SetFloat("ssaoPower", 0.2f);

		_pixelShader->SetShaderResourceView("DepthTex", _deferredBuffers->GetDepthSRV());
		_pixelShader->SetShaderResourceView("NormalTex", _deferredBuffers->GetShaderResourceView(2));
		_pixelShader->SetShaderResourceView("Noise", _noiseSRV.Get());

		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		ResourceManager::Instance().GetDeviceContext()->Draw(4, 0);

		ID3D11ShaderResourceView* nullSRV = nullptr;
		_deviceContext->PSSetShaderResources(0, 1, &nullSRV);
		_deviceContext->PSSetShaderResources(1, 1, &nullSRV);
		_deviceContext->PSSetShaderResources(2, 1, &nullSRV);
	}

	void SSAOPass::CreateTexture(UINT screenWidth, UINT screenHeight)
	{
		_windowSize.x = static_cast<float>(screenWidth);
		_windowSize.y = static_cast<float>(screenHeight);

		_noiseTexture.Reset();
		_noiseSRV.Reset();

		// Create noise texture
		std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
		std::default_random_engine generator;

		for (int i = 0; i < 64; ++i)
		{
			XMVECTOR sample = XMVectorSet(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator),
				0);
			sample = XMVector3Normalize(sample);
			float scale = (float)i / 64.0;
			XMVectorScale(sample, scale);
			XMStoreFloat4(&_ssaoSamples[i], sample);
		}

		using namespace DirectX::PackedVector;
		std::vector<XMBYTEN4> rotation;
		rotation.reserve(16);
		for (int i = 0; i < 16; ++i)
		{
			XMVECTOR rot = XMVectorSet(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				0,
				0);
			XMBYTEN4 rotByte;
			XMStoreByteN4(&rotByte, rot);
			rotation.push_back(rotByte);
		}

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = rotation.data();
		data.SysMemPitch = 4 * 4; // 4 pixels width, 4 bytes (32 bits)

		D3D11_TEXTURE2D_DESC noiseTextureDesc;
		ZeroMemory(&noiseTextureDesc, sizeof(noiseTextureDesc));
		noiseTextureDesc.Width = 4;
		noiseTextureDesc.Height = 4;
		noiseTextureDesc.MipLevels = 1;
		noiseTextureDesc.ArraySize = 1;
		noiseTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;
		noiseTextureDesc.SampleDesc.Count = 1;
		noiseTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		noiseTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		noiseTextureDesc.CPUAccessFlags = 0;
		noiseTextureDesc.MiscFlags = 0;

		ResourceManager::Instance().GetDevice()->CreateTexture2D(&noiseTextureDesc, &data, _noiseTexture.GetAddressOf());

		D3D11_SHADER_RESOURCE_VIEW_DESC noiseSRVDesc;
		noiseSRVDesc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;
		noiseSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		noiseSRVDesc.Texture2D.MostDetailedMip = 0;
		noiseSRVDesc.Texture2D.MipLevels = 1;

		ResourceManager::Instance().GetDevice()->CreateShaderResourceView(_noiseTexture.Get(), &noiseSRVDesc, _noiseSRV.GetAddressOf());
	}

}
