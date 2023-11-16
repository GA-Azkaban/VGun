#include "UIText.h"
#include "MZCamera.h"
#include "Vertex.h"
#include "Effects.h"
#include "ResourceManager.h"
#include "DebugCube.h"
#include "DXTKFont.h"
#include "MZDX11Renderer.h"
#include "DeferredRenderer.h"

UIText::UIText(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS,
	XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 backgroundColor, XMFLOAT4 textColor)
	: m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRenderstate(pRS),
	m_VB(nullptr), m_IB(nullptr), m_isSetScreenSpace(true),
	m_world(XMMatrixIdentity()), m_view(), m_proj(), m_pFont(nullptr),
	m_position(position), m_size(size), m_backgroundColor(backgroundColor), m_textColor(textColor)
{
	BuildGeometryBuffers();
	CreateResources();
	m_pFont = std::make_unique<DXTKFont>();
	m_pFont->Initialize(pDevice, (TCHAR*)L"Font/spaceranger.spritefont");
	m_pFont->SetLineSpacing(20.0f);
	//m_text = (TCHAR*)L"";
}

UIText::~UIText()
{

}

void UIText::SetWorldTM(const XMMATRIX& tm)
{
	m_world = tm;

	m_position.x = m_world.r[3].m128_f32[0];
	m_position.y = m_world.r[3].m128_f32[1];
	m_position.z = m_world.r[3].m128_f32[2];

	m_scale.x = m_world.r[0].m128_f32[0];
	m_scale.y = m_world.r[1].m128_f32[1];
	m_scale.z = m_world.r[2].m128_f32[2];

	m_meshBox.Center.x = m_boundingVolumeInitPos.x + m_world.r[3].m128_f32[0];
	m_meshBox.Center.y = m_boundingVolumeInitPos.y - m_world.r[3].m128_f32[1];
	m_meshBox.Center.z = m_boundingVolumeInitPos.z + m_world.r[3].m128_f32[2];

	m_meshBox.Extents.x = m_boundingVolumeInitExtents.x * m_world.r[0].m128_f32[0];
	m_meshBox.Extents.y = m_boundingVolumeInitExtents.y * m_world.r[1].m128_f32[1];
	m_meshBox.Extents.z = m_boundingVolumeInitExtents.z * m_world.r[2].m128_f32[2];

	m_size = XMFLOAT2(2.0f * m_meshBox.Extents.x, 2.0f * m_meshBox.Extents.y);

	//m_debugCube->SetWorldTM(m_world);
}

void UIText::SetText(TCHAR* text, ...)
{
	TCHAR buffer[1024] = L"";

	va_list vl;
	va_start(vl, text);
	_vstprintf(buffer, 1024, text, vl);
	va_end(vl);

	int index = 0;
	while (m_text[index] != '\0')
	{
		m_text[index++] = '\0';
	}
	index = 0;
	while (buffer[index] != '\0')
	{
		m_text[index++] = buffer[index];
	}
}

const std::string UIText::GetText()
{
	std::wstring wideString(m_text);
	std::string narrowString(wideString.begin(), wideString.end());
	return narrowString;
}

bool UIText::Pick(MZCamera* pCamera, float x, float y)
{
	if (!m_isActive)
		return false;

	if (!m_isSetScreenSpace)
		return false;

	if (!m_isPickingOn)
		return false;

	// x = normalizedX, y = normalizedY ==> 왼쪽상단 (0.0, 0.0) / 오른쪽하단 (1.0, 1.0)
	// screen space X = 2 * x - 1.0f; ==> -1.0 ~ 1.0
	// screen space Y = 2 * y - 1.0f; ==> -1.0 ~ 1.0
	XMVECTOR rayOrigin = XMVectorSet(x * 2 - 1.0f, y * 2 - 1.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(x * 2 - 1.0f, y * 2 - 1.0f, 1.0f, 0.0f);

	float tmin = 0.0f;
	if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &m_meshBox, &tmin))
	{
		m_depth = tmin;
		return true;
	}
	return false;
}

void UIText::Update(MZCamera* pCamera, float deltaTime)
{
	if (!m_isActive)
		return;

	if (m_isSetScreenSpace)
	{
		m_view = XMMatrixIdentity();
		m_proj = XMMatrixIdentity();
	}
	else
	{
		/*m_world.r[0].m128_f32[0] = m_scale.x * pCamera->GetWorld().r[0].m128_f32[0];
		m_world.r[0].m128_f32[2] = pCamera->GetWorld().r[0].m128_f32[2];
		m_world.r[2].m128_f32[0] = pCamera->GetWorld().r[2].m128_f32[0];
		m_world.r[2].m128_f32[2] = m_scale.z * pCamera->GetWorld().r[2].m128_f32[2];

		m_world.r[1].m128_f32[1] = m_scale.y * pCamera->GetWorld().r[1].m128_f32[1];
		m_world.r[1].m128_f32[2] = pCamera->GetWorld().r[1].m128_f32[2];
		m_world.r[2].m128_f32[1] = pCamera->GetWorld().r[2].m128_f32[1];
		m_world.r[2].m128_f32[2] = pCamera->GetWorld().r[2].m128_f32[2];

		m_world.r[0].m128_f32[1] = pCamera->GetWorld().r[0].m128_f32[1];
		m_world.r[1].m128_f32[0] = pCamera->GetWorld().r[1].m128_f32[0];*/

		m_view = pCamera->View();
		m_proj = pCamera->Proj();
	}
	//if (m_debugCube != nullptr)
	//	m_debugCube->Update(XMMatrixIdentity(), XMMatrixIdentity());
}

void UIText::Render()
{
	if (!m_isActive)
		return;

	RenderToTexture();

	DeferredRenderer::Instance.Get().SetRenderTargets();
	
	m_diffuseMapSRV = m_pShaderResourceView;

	// 입력 배치 객체 셋팅
	m_d3dImmediateContext->IASetInputLayout(InputLayouts::TexInputLayout);
	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 세팅
	UINT stride = sizeof(VertexStruct::PosTex);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, m_VB_Quad.GetAddressOf(), &stride, &offset);
	m_d3dImmediateContext->IASetIndexBuffer(m_IB_Quad.Get(), DXGI_FORMAT_R32_UINT, 0);

	// 렌더 스테이트
	m_d3dImmediateContext->RSSetState(m_pRenderstate.Get());

	// world view proj TM 셋팅
	XMMATRIX worldViewProj = m_world * m_view * m_proj;

	ID3DX11EffectTechnique* mTech = Effects::UIObjectFX->DrawUITech;

	// 테크닉
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	// 렌더패스
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		Effects::UIObjectFX->SetWorld(worldViewProj);
		Effects::UIObjectFX->SetDiffuseMap(m_diffuseMapSRV.Get());

		mTech->GetPassByIndex(p)->Apply(0, m_d3dImmediateContext.Get());
		m_d3dImmediateContext->DrawIndexed(6, 0, 0);
	}
}

void UIText::BuildGeometryBuffers()
{
	vertices.resize(4);
	// 정점 버퍼를 만든다.
	vertices[0] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), m_backgroundColor };
	vertices[1] = { XMFLOAT3(-1.0f, 1.0f, 0.0f), m_backgroundColor };
	vertices[2] = { XMFLOAT3(1.0f, 1.0f, 0.0f), m_backgroundColor };
	vertices[3] = { XMFLOAT3(1.0f, -1.0f, 0.0f), m_backgroundColor };

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexStruct::PosColor) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB));

	// 인덱스 버퍼를 만든다.
	UINT indices[] = {
		0,1,2,
		0,2,3,
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));

	vertices_quad.resize(4);
	// 정점 버퍼를 만든다.
	vertices_quad[0] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	vertices_quad[1] = { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
	vertices_quad[2] = { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };
	vertices_quad[3] = { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };

	D3D11_BUFFER_DESC vbd2;
	vbd2.Usage = D3D11_USAGE_IMMUTABLE;
	vbd2.ByteWidth = sizeof(VertexStruct::PosTex) * 4;
	vbd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd2.CPUAccessFlags = 0;
	vbd2.MiscFlags = 0;
	vbd2.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData2;
	vinitData2.pSysMem = &vertices_quad[0];
	HR(m_d3dDevice->CreateBuffer(&vbd2, &vinitData2, &m_VB_Quad));

	// 인덱스 버퍼를 만든다.
	UINT indices2[] = {
		0,1,2,
		0,2,3,
	};

	D3D11_BUFFER_DESC ibd2;
	ibd2.Usage = D3D11_USAGE_IMMUTABLE;
	ibd2.ByteWidth = sizeof(UINT) * 6;
	ibd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd2.CPUAccessFlags = 0;
	ibd2.MiscFlags = 0;
	ibd2.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData2;
	iinitData2.pSysMem = indices2;
	HR(m_d3dDevice->CreateBuffer(&ibd2, &iinitData2, &m_IB_Quad));

	m_size = XMFLOAT2(vertices_quad[3].Pos.x - vertices_quad[0].Pos.x, vertices_quad[2].Pos.y - vertices_quad[0].Pos.y);

	SetBoundingVolume();
}

void UIText::CreateResources()
{
	// Initialize the texture size
	int _textureWidth = MZDX11Renderer::Instance.Get().GetScreenWidth();
	int _textureHeight = MZDX11Renderer::Instance.Get().GetScreenHeight();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = _textureWidth;
	textureDesc.Height = _textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create textures	
	HR(m_d3dDevice->CreateTexture2D(&textureDesc, NULL, m_pRenderTargetTexture.GetAddressOf()));


	// Create render target view to be able to access the render target textures
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	// Create render target views	
	HR(m_d3dDevice->CreateRenderTargetView(m_pRenderTargetTexture.Get(),
		&rtvDesc, m_pRenderTagetView.GetAddressOf()));


	// Create the shader resource views for each texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Create the shader resource views
	HR(m_d3dDevice->CreateShaderResourceView(m_pRenderTargetTexture.Get(),
		&srvDesc, m_pShaderResourceView.GetAddressOf()));


	// Create the depth/stencil buffer and view
	ID3D11Texture2D* _pDepthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.Width = _textureWidth;
	depthBufferDesc.Height = _textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	//depthBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer
	HR(m_d3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &_pDepthStencilBuffer));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.Format = depthBufferDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	dsvDesc.Flags = 0;

	// Create the depth/stencil view
	HR(m_d3dDevice->CreateDepthStencilView(_pDepthStencilBuffer,
		&dsvDesc, m_pDepthStencilView.GetAddressOf()));

	SafeRelease(_pDepthStencilBuffer);
}

void UIText::SetBoundingVolume()
{
	// Bounding box
	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);
	XMVECTOR vMin = XMLoadFloat3(&vMinf3);

	for (UINT i = 0; i < vertices_quad.size(); ++i)
	{
		XMVECTOR P = XMLoadFloat3(&vertices_quad[i].Pos);
		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	XMStoreFloat3(&m_meshBox.Center, 0.5f * (vMin + vMax));
	XMStoreFloat3(&m_meshBox.Extents, 0.5f * (vMax - vMin));
	m_boundingVolumeInitPos = m_meshBox.Center;
	m_boundingVolumeInitExtents = m_meshBox.Extents;
	
	//XMFLOAT3 cubeSize{ 2.0f * m_meshBox.Extents.x, 2.0f * m_meshBox.Extents.y, 2.0f * m_meshBox.Extents.z };
	//m_debugCube = std::unique_ptr<DebugCube>(new DebugCube(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), RasterizerState::WireframeRS.Get(), m_meshBox.Center, cubeSize));
	//m_debugCube->SetWorldTM(m_world);
	//m_debugCube->SetColor(XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
}

void UIText::DrawText()
{
	UINT index = 0;
	UINT length = 0;
	while (m_text[index] != '\0')
	{
		index++;
	}
	length = index;
	float _scale = 40.0f;
	XMVECTORF32 _textColor = { m_textColor.x, m_textColor.y, m_textColor.z, m_textColor.w };
	float posX = MZDX11Renderer::Instance.Get().GetScreenWidth() / 2.0f - (m_fontSize* _scale / 2.4f * length * 1.15f);
	float posY = MZDX11Renderer::Instance.Get().GetScreenHeight() / 2.0f - m_fontSize* _scale / 1.8f;
	m_pFont->DrawText(posX, posY, _scale, _textColor, m_text);
	//m_pFont->DrawText(0, 0, m_textColor, (TCHAR*)L"%d", _hp);
}

void UIText::SetRenderTargets()
{
	// Sets the render targets in the array as the location where the shaders will write to
	m_d3dImmediateContext.Get()->OMSetRenderTargets(1, m_pRenderTagetView.GetAddressOf(), m_pDepthStencilView.Get());
}

void UIText::ClearRenderTargets(const float color[4])
{
	// 렌더 타겟 클리어
	m_d3dImmediateContext.Get()->ClearRenderTargetView(m_pRenderTagetView.Get(), color);
	m_d3dImmediateContext.Get()->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void UIText::RenderToTexture()
{
	SetRenderTargets();
	ClearRenderTargets(DirectX::Colors::Black);

	// 입력 배치 객체 셋팅
	m_d3dImmediateContext->IASetInputLayout(InputLayouts::PosColor);
	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 세팅
	UINT stride = sizeof(VertexStruct::PosColor);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);
	m_d3dImmediateContext->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	// 렌더 스테이트
	m_d3dImmediateContext->RSSetState(m_pRenderstate.Get());

	ID3DX11EffectTechnique* mTech = Effects::UITextFX->DrawUITextTech;

	// 테크닉
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	// 렌더패스
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, m_d3dImmediateContext.Get());
		m_d3dImmediateContext->DrawIndexed(6, 0, 0);
	}
	//if (m_debugCube != nullptr)
	//	m_debugCube->RenderToTexture();
	DrawText();
}