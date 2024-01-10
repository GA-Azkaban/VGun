#include <DXTK/DDSTextureLoader.h>
#include "Texture.h"
#include "RocketMacroDX11.h"

namespace RocketCore::Graphics
{
	Texture::Texture()
		: _texture(nullptr), _textureView(nullptr)
	{

	}

	Texture::Texture(const Texture& other)
	{

	}

	Texture::~Texture()
	{

	}

	bool Texture::Initialize(ID3D11Device* device, const std::wstring& path)
	{
		HR(DirectX::CreateDDSTextureFromFile(device, path.c_str(), &_texture, &_textureView));

		return true;
	}

	void Texture::Shutdown()
	{
		// 텍스쳐 뷰 리소스 해제
		if (_textureView)
		{
			_textureView->Release();
			_textureView = nullptr;
		}

		if (_texture)
		{
			_texture->Release();
			_texture = nullptr;
		}
	}

	ID3D11ShaderResourceView* Texture::GetTexture()
	{
		return _textureView;
	}

}