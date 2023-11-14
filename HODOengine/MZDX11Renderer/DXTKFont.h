#pragma once
#include "SpriteFont.h"
#include "SpriteBatch.h"

/// <summary>
/// DXTK를 이용한 텍스트 출력
/// 
/// 2023.06.28 MJKIM
/// </summary>

class DXTKFont
{
public:
	DXTKFont();
	~DXTKFont();

	void Initialize(ID3D11Device* pDevice, TCHAR* fileName);
	void Initialize(ID3D11Device* pDevice, ID3D11RasterizerState* rs, ID3D11DepthStencilState* ds);
	void DrawText(int x, int y, float scale, DirectX::XMVECTORF32 color, TCHAR* text, ...);

public:
	float GetLineSpacing() {
		if (!m_pSpriteFont) return 0.0f;
		return m_pSpriteFont->GetLineSpacing();
	}
	void SetLineSpacing(float lineSpace) {
		if (m_pSpriteFont != nullptr)
			m_pSpriteFont->SetLineSpacing(lineSpace);
	}

private:
	DirectX::SpriteBatch* m_pSpriteBatch;
	DirectX::SpriteFont* m_pSpriteFont;

	ID3D11RasterizerState* m_rasterizerState;
	ID3D11DepthStencilState* m_depthStencilState;
};

