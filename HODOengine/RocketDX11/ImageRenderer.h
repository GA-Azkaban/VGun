#pragma once

#include <d3d11.h>
#include <memory>
#include <DXTK\\SpriteBatch.h>
#include <DXTK\\WICTextureLoader.h>
#include <wrl\client.h>

#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"
#include "MathHeader.h"

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;
	class VertexShader;
	class PixelShader;

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

		virtual void SetAngle(float angle) override;

		virtual void SetOrigin(float x, float y) override;

		virtual void SetCenter(float x, float y) override;

		virtual float GetWidth() override;

		virtual float GetHeight() override;

		virtual void SetSortOrder(float order) override;
		float GetSortOrder() const { return _sortOrder; }

		virtual void SetDefalutColor(DirectX::FXMVECTOR color) override;
		virtual void RetunDefalutColor()override;

		void InitalizeImageRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		Vector2 GetImageScale();

		// fade
		virtual bool SetFadeMode(bool SetFade,float time) override;
		virtual void FadeIn(float time) override;
		virtual void FadeOut(float time) override;
		virtual bool GetFadeMode() override;
		virtual bool GetComplete() override;

	public:
		void Render(DirectX::SpriteBatch* spriteBatch);

	private:
		// 내부 변수들
		std::string _str;
		DirectX::XMVECTOR _color;
		DirectX::XMVECTOR _defalutcolor;
		DirectX::XMFLOAT2 _origin;
		float _xlocation;
		float _ylocation;
		float _scaleX;
		float _scaleY;
		float _rotationRadian;
		bool _active;
		bool _receiveTMInfoFlag;

		// 원점 구하기
		float _centerX;
		float _centerY;
		DirectX::XMMATRIX translationMatrix;

		// 이미지 정보
		float _currentImageWidth;
		float _originalImageWidth;
		float _currentImageHeight;
		float _originalImageHeight;

		// 페이드 인 & 아웃
		bool _fadeMode = true;
		bool _isComplete = false;
		float _fadeAlpha = 1.0f;
		float _fadeDuration;	// 페이드인 아웃에 걸리는시간
		float _fadeIncrement;	// 한 프레임에서 증가할 알파 값

		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _imagerSRV;
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _textureSRV;

		float _sortOrder;

		// world space UI
		bool _isInWorldSpace;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> _rasterizerState;
		DirectX::XMMATRIX _world;
		Mesh* _mesh;
		Material* _material;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
	};
}
