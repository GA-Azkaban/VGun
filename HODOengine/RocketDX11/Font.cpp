#include "Font.h"

#include <DXTK/SimpleMath.h>
#include <DXTK/DDSTextureLoader.h>

namespace RocketCore::Graphics
{

	Font::Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	{
		m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(pDeviceContext);
		m_SpriteFont = std::make_unique<DirectX::SpriteFont>(pDevice, L"..\\Font\\NotoSansKR.spritefont");
		//m_SpriteBatch = new DirectX::SpriteBatch(pDeviceContext);
		//m_SpriteFont = new DirectX::SpriteFont(pDevice, L"..\\Font\\NotoSansKR.spritefont");
	}

	Font::~Font()
	{

	}

	void Font::RenderString(const std::string& str, float x, float y)
	{
 		DirectX::FXMVECTOR color = DirectX::Colors::White;
 		std::wstring wstr(str.begin(), str.end());
 		m_SpriteBatch->Begin();
 		m_SpriteFont->DrawString(m_SpriteBatch.get(), wstr.c_str(), DirectX::XMFLOAT2(x, y), color);
 		m_SpriteBatch->End();
	}

	void Font::RenderString(const float floatValue, float x, float y)
	{
		DirectX::FXMVECTOR color = DirectX::Colors::White;
		std::string str = std::to_string(floatValue);
		std::wstring wstr(str.begin(), str.end());
		m_SpriteBatch->Begin();
		m_SpriteFont->DrawString(m_SpriteBatch.get(), wstr.c_str(), DirectX::XMFLOAT2(x, y), color);
 		m_SpriteBatch->End();
	}

	void Font::RenderString(const int intValue, float x, float y)
	{
		DirectX::FXMVECTOR color = DirectX::Colors::White;
		std::string str = std::to_string(intValue);
		std::wstring wstr(str.begin(), str.end());
		m_SpriteBatch->Begin();
		m_SpriteFont->DrawString(m_SpriteBatch.get(), wstr.c_str(), DirectX::XMFLOAT2(x, y), color);
 		m_SpriteBatch->End();
	}

}