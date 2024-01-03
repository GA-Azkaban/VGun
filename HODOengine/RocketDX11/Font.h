#pragma once

#include <d3d11.h>
#include <memory>
#include <DirectXColors.h>
#include <string>
#include <DXTK/SpriteBatch.h>
#include <DXTK/SpriteFont.h>

//#pragma comment(lib,"..\\Lib\\x64\\Debug\\DirectXTK.lib")
       

namespace RocketCore::Graphics
{
	class Font
	{
	public:
		Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
		~Font();

		void RenderString(const std::string& str, float x, float y);
		void RenderString(const float str, float x, float y);
		void RenderString(const int str, float x, float y);

	private:
		// DXTk 내부 클래스
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> m_SpriteFont;
		//DirectX::SpriteBatch* m_SpriteBatch;
		//DirectX::SpriteFont* m_SpriteFont;

	};
}