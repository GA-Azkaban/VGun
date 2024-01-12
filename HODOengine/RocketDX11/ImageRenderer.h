#pragma once

#include <d3d11.h>
#include <memory>
//#include <vector>
#include <DXTK\\SpriteBatch.h>
#include <DXTK\\WICTextureLoader.h>
#include <wrl\client.h>

#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"
#include "..\\HODOmath\\HODOmath.h"



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

		virtual void SetScereenSpace() override;

		virtual void SetWorldSpace() override;

		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& worldTM) override;

		virtual void SetActive(bool isActive) override;

		virtual void ChangeScale(float x, float y) override;

		void InitalizeImageRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);


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

		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _imagerSRV;
<<<<<<< HEAD
=======
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _textureSRV;

		bool _isTranslated;
>>>>>>> ab573bef3ee680cc888d7abe33ca669fae428aea
	};
}