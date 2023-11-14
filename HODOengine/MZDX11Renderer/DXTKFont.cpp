#include "DXTKFont.h"
#include <stdarg.h>
#include <tchar.h>

DXTKFont::DXTKFont()
	: m_pSpriteBatch(nullptr), m_pSpriteFont(nullptr)
{

}

DXTKFont::~DXTKFont()
{
	delete m_pSpriteFont;
	delete m_pSpriteBatch;
}

void DXTKFont::Initialize(ID3D11Device* pDevice, TCHAR* fileName)
{
	ID3D11DeviceContext* pDC = nullptr;
	pDevice->GetImmediateContext(&pDC);

	m_pSpriteBatch = new DirectX::SpriteBatch(pDC);

	//TCHAR* _fileName = (TCHAR*)L"Font/gulim9k.spritefont";
	//TCHAR* _fileName = (TCHAR*)L"Font/spaceranger.spritefont";

	m_pSpriteFont = new DirectX::SpriteFont(pDevice, fileName);

	pDC->Release();
}

void DXTKFont::Initialize(ID3D11Device* pDevice, ID3D11RasterizerState* rs, ID3D11DepthStencilState* ds)
{
	ID3D11DeviceContext* pDC = nullptr;
	pDevice->GetImmediateContext(&pDC);

	m_pSpriteBatch = new DirectX::SpriteBatch(pDC);

	TCHAR* _fileName = (TCHAR*)L"Font/gulim9k.spritefont";
	//TCHAR* _fileName = (TCHAR*)L"Font/spaceranger.spritefont";

	m_pSpriteFont = new DirectX::SpriteFont(pDevice, _fileName);

	m_rasterizerState = rs;
	m_depthStencilState = ds;

	delete pDC;
}

void DXTKFont::DrawText(int x, int y, float scale, DirectX::XMVECTORF32 color, TCHAR* text, ...)
{
	TCHAR buffer[1024] = L"";

	va_list vl;
	va_start(vl, text);
	_vstprintf(buffer, 1024, text, vl);
	va_end(vl);

	//m_pSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, m_depthStencilState, m_rasterizerState);
	m_pSpriteBatch->Begin();
	m_pSpriteFont->DrawString(m_pSpriteBatch, buffer, DirectX::XMFLOAT2((float)x, (float)y), color, 0.0f, DirectX::XMFLOAT2(0,0), scale);
	m_pSpriteBatch->End();
}
