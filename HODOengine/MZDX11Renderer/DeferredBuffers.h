#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"

// Render Target Type
enum class BUFFERTYPE
{
	BUFFER_POSITION,
	BUFFER_NORNAL,
	BUFFER_DIFFUSE,
	BUFFER_TANGENT,
	BUFFER_NORMALMAP,
	//BUFFER_DEPTH,

	GBUFFER_COUNT,
};

class DeferredBuffers
{
public:
	~DeferredBuffers();
	static MZRenderer::LazyObjects<DeferredBuffers> Instance;
	friend MZRenderer::LazyObjects<DeferredBuffers>;

	void Initialize(ID3D11Device* device, int textureWidth, int textureHeight);
	void Finalize();

	void SetRenderTargets(ID3D11DeviceContext* deviceContext);
	void ClearRenderTargets(ID3D11DeviceContext* deviceContext, XMVECTOR color);

	inline ID3D11ShaderResourceView* GetShaderResourceView(int iRenderTargetIndex)
	{
		if (iRenderTargetIndex < static_cast<int>(BUFFERTYPE::GBUFFER_COUNT))
		{
			return m_pShaderResourceViews[iRenderTargetIndex].Get();
		}
		else
		{
			return nullptr;
		}
	}
	/*inline ID3D11ShaderResourceView* GetZBufferShaderResourceView()
	{
		return m_pZBufferRSV;
	}*/


private:
	DeferredBuffers();

	int m_textureWidth;
	int m_textureHeight;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pRenderTargetTextures[(int)BUFFERTYPE::GBUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTagetViews[(int)BUFFERTYPE::GBUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceViews[(int)BUFFERTYPE::GBUFFER_COUNT];
	// Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pZBufferRSV;
	// Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

};

