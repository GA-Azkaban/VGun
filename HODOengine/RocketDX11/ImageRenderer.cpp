#include <locale>
#include <codecvt>
#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"
#include "Material.h"
#include "ObjectManager.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"

using namespace DirectX;

RocketCore::Graphics::ImageRenderer::ImageRenderer()
	: _xlocation(),
	_ylocation(),
	_scaleX(1.0f),
	_scaleY(1.0f),
	_imageWidth(),
	_imageHeight(),
	_active(true),
	_receiveTMInfoFlag(false),
	_sortOrder(),
	_isInWorldSpace(false),
	_world(XMMatrixIdentity())
{
	_color = DirectX::Colors::White;

	_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
	_vertexShader = ResourceManager::Instance().GetVertexShader("BillboardVertexShader.cso");
	_pixelShader = ResourceManager::Instance().GetPixelShader("BillboardPixelShader.cso");
	_mesh = ResourceManager::Instance().GetMeshes("primitiveQuad")[0];
	HDEngine::MaterialDesc desc;
	desc.materialName = "UIImage";
	_material = ObjectManager::Instance().CreateMaterial(desc);
	_vertexShader = ResourceManager::Instance().GetVertexShader("BillboardVertexShader.cso");
	_pixelShader = ResourceManager::Instance().GetPixelShader("BillboardPixelShader.cso");
}

RocketCore::Graphics::ImageRenderer::~ImageRenderer()
{

}

void RocketCore::Graphics::ImageRenderer::SetImage(const std::string& filePath)
{
	_material->LoadAlbedoTexture(filePath);

	D3D11_TEXTURE2D_DESC textureDesc;
	ID3D11Resource* resource;
	_material->GetAlbedoMap()->GetResource(&resource);
	//_imagerSRV->GetResource(&resource);
	ID3D11Texture2D* texture2D = static_cast<ID3D11Texture2D*>(resource);
	texture2D->GetDesc(&textureDesc);

	_imageWidth = textureDesc.Width;
	_imageHeight = textureDesc.Height;

	_centerX = _imageWidth / 2.0f;
	_centerY = _imageHeight / 2.0f;
}

void RocketCore::Graphics::ImageRenderer::SetScreenSpacePosition(float x, float y)
{
	_xlocation;
	_ylocation;
	//_xlocation = x - _centerX;
	//_ylocation = y - _centerY;
}

void RocketCore::Graphics::ImageRenderer::SetWorldSpace()
{
	_isInWorldSpace = true;
}

void RocketCore::Graphics::ImageRenderer::Render(DirectX::SpriteBatch* spriteBatch)
{
	if (!_active)
		return;

	if (_fadeMode)
	{
		if (_fadeAlpha <= 1.0f)
		{
			_fadeAlpha += 0.002f;
			if (_fadeAlpha > 1.0f)
			{
				_fadeAlpha = 1.0f;
				_isComplete = true;
			}
			//_color = DirectX::XMVECTOR{_color.x,_color.y,_color.z,_fadeAlpha };
			// _color의 RGB 채널을 유지하면서 알파 채널을 _fadeAlpha 값으로 변경
			_color = DirectX::XMVectorSetW(_color, _fadeAlpha);

		}
	}
	else
	{
		if (_fadeAlpha >= 0.0f)
		{
			_fadeAlpha -= 0.002f;
			if (_fadeAlpha < 0.0f)
			{
				_fadeAlpha = 0.0f;
				_isComplete = false;
			}
			//_color = DirectX::XMVECTOR{ 1.0f,1.0f,1.0f,_fadeAlpha };
			// _color의 RGB 채널을 유지하면서 알파 채널을 _fadeAlpha 값으로 변경
			_color = DirectX::XMVectorSetW(_color, _fadeAlpha);

		}
	}

	if (!_fadeMode && _fadeAlpha == 0.0f)
	{
		return;
	}

	if (_receiveTMInfoFlag)
	{
		if (_isInWorldSpace)
		{
			ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			ResourceManager::Instance().GetDeviceContext()->RSSetState(_rasterizerState.Get());

			_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(_world));

			_vertexShader->CopyAllBufferData();
			_vertexShader->SetShader();

			_pixelShader->SetShaderResourceView("Albedo", _material->GetAlbedoMap());
			XMFLOAT4 albedoColor; 
			XMStoreFloat4(&albedoColor, _color);
			_pixelShader->SetFloat4("albedoColor", albedoColor);

			_pixelShader->CopyAllBufferData();
			_pixelShader->SetShader();

			_mesh->BindBuffers();
			_mesh->Draw();
		}
		else
		{
			spriteBatch->Draw(
				_material->GetAlbedoMap(),
				DirectX::XMFLOAT2(_xlocation - _centerX, _ylocation - _centerY),
				nullptr,
				_color,
				0.0f,										//회전 각도
				DirectX::XMFLOAT2(0.5f, 0.5f),				//  이미지의 원점->0.0f,0.0f이면 좌측상단
				DirectX::XMFLOAT2(_scaleX, _scaleY),		// 이미지 스케일
				DirectX::DX11::SpriteEffects_None,
				_sortOrder
			);
		}
	}

	_receiveTMInfoFlag = false;
}

void RocketCore::Graphics::ImageRenderer::SetWorldTM(const Matrix& worldTM)
{
	if (_isInWorldSpace)
	{
		// 카메라 방향으로 회전해준다
		XMFLOAT3 cameraPosition = Camera::GetMainCamera()->GetPosition();
		float radian = std::atan2(0.0 - cameraPosition.x, 0.0 - cameraPosition.z);
		XMVECTOR translate;
		XMVECTOR rotation;
		XMVECTOR scale;
		XMMatrixDecompose(&scale, &rotation, &translate, worldTM);
		_world = XMMatrixScalingFromVector(scale) * XMMatrixRotationRollPitchYawFromVector(rotation) * XMMatrixRotationY(radian) * XMMatrixTranslationFromVector(translate);
	}
	else
	{
		_xlocation = worldTM._41;
		_ylocation = worldTM._42;
	}
	_receiveTMInfoFlag = true;
}

void RocketCore::Graphics::ImageRenderer::SetScreenSpace()
{
	_isInWorldSpace = false;
}

void RocketCore::Graphics::ImageRenderer::SetActive(bool isActive)
{
	_active = isActive;
}

void RocketCore::Graphics::ImageRenderer::InitalizeImageRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	_device = device;
	_deviceContext = deviceContext;
}

void RocketCore::Graphics::ImageRenderer::ChangeScale(float x, float y)
{
	_scaleX = x;
	_scaleY = y;
	_imageWidth *= _scaleX;
	_imageHeight *= _scaleY;
}

float RocketCore::Graphics::ImageRenderer::GetScreenSpacePositionX()
{
	return _xlocation;
}

float RocketCore::Graphics::ImageRenderer::GetScreenSpacePositionY()
{
	return _ylocation;
}

float RocketCore::Graphics::ImageRenderer::GetWidth()
{
	return _imageWidth;
}

float RocketCore::Graphics::ImageRenderer::GetHeight()
{
	return _imageHeight;
}

DirectX::FXMVECTOR RocketCore::Graphics::ImageRenderer::SetColor(DirectX::FXMVECTOR color)
{
	return _color = color;
}

void RocketCore::Graphics::ImageRenderer::SetSortOrder(float order)
{
	_sortOrder = order;
}

void RocketCore::Graphics::ImageRenderer::FadeIn()
{
	_fadeMode = true;
}

void RocketCore::Graphics::ImageRenderer::FadeOut()
{
	_fadeMode = false;
}

bool RocketCore::Graphics::ImageRenderer::GetFadeMode()
{
	return _fadeMode;
}

bool RocketCore::Graphics::ImageRenderer::SetFadeMode(bool SetFade)
{
	return _fadeMode = SetFade;
}

bool RocketCore::Graphics::ImageRenderer::GetComplete()
{
	return _isComplete;
}

void RocketCore::Graphics::ImageRenderer::SetDefalutColor(DirectX::FXMVECTOR color)
{
	_defalutcolor = color;
}

void RocketCore::Graphics::ImageRenderer::RetunDefalutColor()
{
	_color = _defalutcolor;
}
 
