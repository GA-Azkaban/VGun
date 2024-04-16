#pragma once

#include <d3d11.h>
#include <memory>
//#include <vector>
#include <DXTK\\SpriteBatch.h>
#include <DXTK\\WICTextureLoader.h>
#include <wrl\client.h>

#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"
#include "MathHeader.h"



namespace RocketCore::Graphics
{
	class ImageRenderer : public HDEngine::ISketchableImage
	{
	public:
		ImageRenderer();
		~ImageRenderer();

		virtual void SetImage(const std::string& filePath) override;

		virtual void SetScreenSpacePosition(float x, float y) override;

		virtual float GetScreenSpacePositionX() override;
		virtual float GetScreenSpacePositionY() override;

		virtual DirectX::FXMVECTOR SetColor(DirectX::FXMVECTOR color) override;

		virtual void SetScreenSpace() override;

		virtual void SetWorldSpace() override;

		virtual void SetWorldTM(const Matrix& worldTM) override;

		virtual void SetActive(bool isActive) override;

		virtual void ChangeScale(float x, float y) override;

		virtual float GetWidth() override;

		virtual float GetHeight() override;

		virtual void SetSortOrder(float order) override;
		float GetSortOrder() const { return _sortOrder; }



		void InitalizeImageRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		// fade
		virtual void FadeIn() override;
		virtual void FadeOut() override;
		virtual bool GetFadeMode() override;
		virtual bool SetFadeMode(bool SetFade) override;

	public:
		void Render(DirectX::SpriteBatch* spriteBatch);

	private:
		// 내부 변수들
		std::string _str;
		DirectX::XMVECTOR _color;
		float _xlocation;
		float _ylocation;
		float _scaleX;
		float _scaleY;
		bool _active;
		bool _receiveTMInfoFlag;

		// 원점 구하기
		float _centerX;
		float _centerY;
		DirectX::XMMATRIX translationMatrix;

		// 이미지 정보
		float _imageWidth;
		float _imageHeight;

		// 페이드 인 & 아웃
		bool _fadeMode = true;
		float _fadeAlpha = 1.0f;

		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _imagerSRV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _textureSRV;

		float _sortOrder;
	};
}
