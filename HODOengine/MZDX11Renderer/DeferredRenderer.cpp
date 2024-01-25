//#include "DeferredRenderer.h"
//#include "MZDX11Renderer.h"
//#include "DeferredBuffers.h"
//#include "ResourceManager.h"
//#include "RasterizerState.h"
//#include "SamplerState.h"
//#include "MZMacro.h"
//#include "Vertex.h"
//#include "Mesh.h"
//#include "Material.h"
//#include "GeometryGenerator.h"
//#include "Lights.h"
//#include "DXTKFont.h"
//#include "MZCamera.h"
//#include "ResourceManager.h"
//#include "StaticMeshObject.h"
//#include "SkinningMeshObject.h"
//#include "HelperObject.h"
//#include "Sky.h"
//
//LazyObjects<DeferredRenderer> DeferredRenderer::Instance;
//
//DeferredRenderer::DeferredRenderer()
//	: m_d3dDevice(0), m_d3dDeviceContext(0), 
//	m_depthStencilStateEnable(0), m_depthStencilStateDisable(0),
//	switcher(0)
//{
//	ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));
//	m_font = new DXTKFont();
//}
//
//DeferredRenderer::~DeferredRenderer()
//{
//	DeferredBuffers::Instance.Get().Finalize();
//
//	m_depthStencilStateDisable->Release();
//	m_depthStencilStateEnable->Release();
//
//	delete m_font;
//}
//
//void DeferredRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain, unsigned int screenWidth, unsigned int screenHeight)
//{
//	m_d3dDevice = device;
//	m_d3dDeviceContext = deviceContext;
//	m_swapChain = swapChain;
//	m_screenWidth = screenWidth;
//	m_screenHeight = screenHeight;
//
//	GetAdapterInfo();
//
//	// Set font
//	m_font->Initialize(m_d3dDevice.Get(), (TCHAR*)L"../x64/Debug/Font/gulim9k.spritefont");
//	m_font->SetLineSpacing(15.0f);
//
//	ResizeBuffers();
//
//	CreateDepthStecilStates();
//
//	SetLights();
//
//	DeferredBuffers::Instance.Get().Initialize(m_d3dDevice.Get(), m_screenWidth, m_screenHeight);
//
//	HelperObject* grid = new HelperObject();
//	grid->SetMesh("grid");
//
//	HelperObject* axis = new HelperObject();
//	axis->SetMesh("axis");
//
//	// FBX Test
//	ResourceManager::Instance.Get().LoadFBXFile("../3DModels/Rob02.fbx");
//	ResourceManager::Instance.Get().LoadFBXFile("../3DModels/4QCharacter_tpose.fbx");
//	ResourceManager::Instance.Get().LoadFBXFile("../3DModels/4QCharacter_idle_ani.fbx");
//	ResourceManager::Instance.Get().LoadFBXFile("../3DModels/A_TP_CH_Sprint_F.fbx");
//	ResourceManager::Instance.Get().LoadFBXFile("../3DModels/model.dae");
//	ResourceManager::Instance.Get().LoadTextureFile("../3DModels/Textures/sunsetcube1024.dds");
//	ResourceManager::Instance.Get().LoadTextureFile("../3DModels/Textures/diffuse.png");
//
//	//StaticMeshObject* test1 = new StaticMeshObject();
//	//SkinningMeshObject* test1 = new SkinningMeshObject();
//	test1 = new SkinningMeshObject();
//	//test1->SetMesh("Rob02.fbx");
//	test1->SetMesh("A_TP_CH_Sprint_F.fbx");	
//	test1->SetDiffuseTexture("Rob02Yellow_AlbedoTransparency.png");
//	test1->SetNormalTexture("Rob02White_Normal.png");
//	//test1->PlayAnimation(1, false);
//	test1->PlayAnimation(0, true);
//
//	sky = new Sky();
//	sky->SetMesh("skySphere");
//	sky->SetCubeMapTexture("sunsetcube1024.dds");
//
//	// Texture Boxes
//	/*for (int i = 0; i < 5; ++i)
//	{
//		TextureBox* box = new TextureBox(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), RasterizerState::SolidRS.Get());
//		m_textureBoxes.emplace_back(box);
//	}*/
//
//	BuildQuadBuffers();
//
//	// Set the depth stencil state
//	//EnableZBuffering();
//	SetCubemapDSS();
//}
//
//void DeferredRenderer::Update(float deltaTime)
//{
//	m_deltaTime = deltaTime;
//
//	if (GetAsyncKeyState('1') & 0x8000)
//	{
//		test1->PlayAnimation(0, true);
//	}
//	if (GetAsyncKeyState('2') & 0x8000)
//	{
//		test1->PlayAnimation(1, false);
//	}
//
//	for (auto& object : IMeshObject::meshObjects)
//	{
//		object->Update(deltaTime);
//	}
//	/*for (auto& object : IDebugObject::debugObjects)
//	{
//		object->Update(deltaTime);
//	}
//	for (auto& object : IUIObject::uiObjects)
//	{
//		object->Update(deltaTime);
//	}*/
//}
//
//void DeferredRenderer::RenderToBackBuffer()
//{
//	EnableZBuffering();
//
//	//DeferredBuffers::Instance.Get().SetRenderTargets(m_d3dDeviceContext.Get());
//	//DeferredBuffers::Instance.Get().ClearRenderTargets(m_d3dDeviceContext.Get(), DirectX::Colors::Black);
//
//	//// Render Target들에 텍스처들을 저장한다.
//	//for (auto object : IMeshObject::meshObjects)
//	//{
//	//	object->Render();
//	//}
//
//	// 최종적으로 하나의 Render Target에 모두 합쳐서 그린다.
//	SetRenderTarget(m_backBufferRTV.Get(), m_depthStencilView.Get());
//	ClearRenderTarget(m_backBufferRTV.Get(), m_depthStencilView.Get(), reinterpret_cast<const float*>(&DirectX::Colors::Black));
//	//DisableZBuffering();
//
//	//RenderAll();
//
//	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	m_d3dDeviceContext->RSSetState(RasterizerState::Instance.Get().GetSolidRS());
//	for (auto object : IMeshObject::meshObjects)
//	{
//		object->Render();
//	}
//
//	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//	m_d3dDeviceContext->RSSetState(RasterizerState::Instance.Get().GetWireframeRS());
//	for (auto object : IDebugObject::debugObjects)
//	{
//		object->Render();
//	}
//
//	SetCubemapDSS();
//	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	m_d3dDeviceContext->RSSetState(RasterizerState::Instance.Get().GetCubeMapRS());
//	sky->Render();
//
//	/*for (auto& object : IUIObject::uiObjects)
//	{
//		object->Render();
//	}*/
//
//	// 디버그용 텍스처 박스 출력
//	/*for (int i = 0; i < 5; ++i)
//	{
//		m_textureBoxes[i]->SetTexture(DeferredBuffers::Instance.Get().GetShaderResourceView(i));
//		if (switcher == 1)
//			m_textureBoxes[i]->Render();
//	}*/
//
//	DrawStatus();
//
//	/*ID3D11ShaderResourceView* shaderResView = NULL;
//	m_d3dDeviceContext->PSSetShaderResources(0, 1, &shaderResView);
//	m_d3dDeviceContext->PSSetShaderResources(1, 1, &shaderResView);
//	m_d3dDeviceContext->PSSetShaderResources(2, 1, &shaderResView);
//	m_d3dDeviceContext->PSSetShaderResources(3, 1, &shaderResView);
//	m_d3dDeviceContext->PSSetShaderResources(4, 1, &shaderResView);*/
//}
//
//void DeferredRenderer::ResizeResolution(unsigned int screenWidth, unsigned int screenHeight)
//{
//	m_screenWidth = screenWidth;
//	m_screenHeight = screenHeight;
//
//	ResizeBuffers();
//
//	DeferredBuffers::Instance.Get().Initialize(m_d3dDevice.Get(), m_screenWidth, m_screenHeight);
//}
//
//void DeferredRenderer::RenderAll()
//{
//	//// 입력 배치 객체 셋팅
//	//m_d3dDeviceContext->IASetInputLayout(InputLayouts::DeferredInputLayout);
//
//	//m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//// 렌더 스테이트
//	//m_d3dDeviceContext->RSSetState(RasterizerState::SolidRS.Get());
//
//	//// Set per frame constants.
//	//Effects::PBRFX->SetDirLights(m_dirLights);
//	//Effects::PBRFX->SetPointLights(m_pointLights);
//	//Effects::PBRFX->SetSpotLights(m_spotLights);
//	//Effects::PBRFX->SetEyePosW(mEyePosW);
//	////Effects::PBRFX->SetMaterial(m_material);
//
//	//// Figure out which technique to use.
//	//ID3DX11EffectTechnique* mTech = Effects::PBRFX->ComputeLightingTech;
//
//	//// 테크닉은...
//	//D3DX11_TECHNIQUE_DESC techDesc;
//	//mTech->GetDesc(&techDesc);
//	//UINT stride = sizeof(VertexStruct::PosTex);
//	//UINT offset = 0;
//	//m_d3dDeviceContext->IASetVertexBuffers(0, 1, &m_QuadVB, &stride, &offset);
//	//m_d3dDeviceContext->IASetIndexBuffer(m_QuadIB, DXGI_FORMAT_R32_UINT, 0);
//	//Effects::PBRFX->SetShaderResource(DeferredBuffers::Instance.Get().GetShaderResourceView(0), 0);
//	//Effects::PBRFX->SetShaderResource(DeferredBuffers::Instance.Get().GetShaderResourceView(1), 1);
//	//Effects::PBRFX->SetShaderResource(DeferredBuffers::Instance.Get().GetShaderResourceView(2), 2);
//	//Effects::PBRFX->SetShaderResource(DeferredBuffers::Instance.Get().GetShaderResourceView(3), 3);
//	//Effects::PBRFX->SetShaderResource(DeferredBuffers::Instance.Get().GetShaderResourceView(4), 4);
//
//	//mTech->GetPassByIndex(0)->Apply(0, m_d3dDeviceContext.Get());
//	//m_d3dDeviceContext->DrawIndexed(6, 0, 0);
//
//}
//
//void DeferredRenderer::DrawStatus()
//{
//	DirectX::XMVECTORF32 _color = { 1.f, 1.f, 1.f, 1.f };
//	float _fontLineSpace = m_font->GetLineSpacing();
//
//	// FPS, DeltaTime
//	static float accumulatedTime = 0.0f;
//	static float fps = 0.0f;
//	static float deltaTimeMS = 0.0f;
//
//	// 측정 주기는 0.2초
//	if (accumulatedTime > 0.2f)
//	{
//		fps = 1.0f / m_deltaTime;
//		deltaTimeMS = m_deltaTime * 1000.0f;
//		accumulatedTime = 0.0f;
//	}
//	int _yPos = 10;
//
//	accumulatedTime += m_deltaTime;
//	m_font->DrawText(0, _yPos, 1.0f, _color, (TCHAR*)L"FPS : %.2f", fps);
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"DeltaTime : %.4f ms", deltaTimeMS);
//
//	// 피쳐레벨
//	m_font->DrawText(0, _yPos += 2 * _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"Feature Level : %x", m_d3dDevice->GetFeatureLevel());
//
//	// 어댑터 정보
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"Description: %s", m_AdapterDesc1.Description);
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"VendorID: %u", m_AdapterDesc1.VendorId);
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"DeviceID: %u", m_AdapterDesc1.DeviceId);
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"SubSysID: %u", m_AdapterDesc1.SubSysId);
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"Revision: %u", m_AdapterDesc1.Revision);
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"VideoMemory: %lu MB", m_AdapterDesc1.DedicatedVideoMemory / 1024 / 1024);
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"SystemMemory: %lu MB", m_AdapterDesc1.DedicatedSystemMemory / 1024 / 1024);
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"SharedSysMemory: %lu MB", m_AdapterDesc1.SharedSystemMemory / 1024 / 1024);
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"AdpaterLuid: %u.%d", m_AdapterDesc1.AdapterLuid.HighPart, m_AdapterDesc1.AdapterLuid.LowPart);
//
//	// 클라이언트 영역의 해상도
//	m_font->DrawText(0, _yPos += 2 * _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"Client Resolution: %dx%d", static_cast<int>(m_screenWidth), static_cast<int>(m_screenHeight));
//
//	// 카메라 정보
//	m_font->DrawText(0, _yPos += _fontLineSpace + 2.5f, 1.0f, _color, (TCHAR*)L"Camera Pos : %.2f / %.2f / %.2f", MZCamera::GetMainCamera()->GetPosition().x, MZCamera::GetMainCamera()->GetPosition().y, MZCamera::GetMainCamera()->GetPosition().z);
//}
//
//IRenderableObject* DeferredRenderer::Pick(float normalizedX, float normalizedY)
//{
//	//float _screenCoordX = normalizedX * m_screenWidth;
//	//float _screenCoordY = normalizedY * m_screenHeight;
//	//XMMATRIX P = MZCamera::GetMainCamera()->Proj();
//
//	//// Compute picking ray in view space.
//	//// 2장과 같은 이유로 P(0,0) -> P.r[0].m128_f32[0]
//	//// 2장과 같은 이유로 P(1,1) -> P.r[1].m128_f32[1]
//	//float vx = (+2.0f * _screenCoordX / m_screenWidth - 1.0f) / P.r[0].m128_f32[0];
//	//float vy = (-2.0f * _screenCoordY / m_screenHeight + 1.0f) / P.r[1].m128_f32[1];
//
//	//pickedObjects.clear();
//
//	//for (auto& object : IUIObject::uiObjects)
//	//{
//	//	if (object->Pick(normalizedX, normalizedY))
//	//	{
//	//		float depth = object->GetDepth();
//	//		pickedObjects.insert(std::make_pair(depth, object));
//	//	}		
//	//}
//
//	//if (!pickedObjects.empty())
//	//{
//	//	return pickedObjects.begin()->second;
//	//}
//
//	//for (auto& object : IMeshObject::meshObjects)
//	//{
//	//	if (object->Pick(vx, vy))
//	//	{
//	//		float depth = object->GetDepth();
//	//		pickedObjects.insert(std::make_pair(depth, object));
//	//	}
//	//}
//
//	//if (!pickedObjects.empty())
//	//{
//	//	return pickedObjects.begin()->second;
//	//}
//
//	return nullptr;
//}
//
//void DeferredRenderer::ResizeBuffers()
//{
//	assert(m_d3dDeviceContext);
//	assert(m_d3dDevice);
//	assert(m_swapChain);
//
//	m_backBufferRTV.Reset();
//	m_depthStencilView.Reset();
//	m_depthStencilBuffer.Reset();
//
//	//swap chain resize & recreate render target view
//	HR(m_swapChain->ResizeBuffers(1, m_screenWidth, m_screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
//	D3D11_TEXTURE2D_DESC bTextureDescription
//	{
//		.Width = m_screenWidth,
//		.Height = m_screenHeight,
//		.MipLevels = 1,
//		.ArraySize = 1,
//		.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
//		.SampleDesc{.Count = 1, .Quality = 0 },
//		.Usage = D3D11_USAGE_DEFAULT,
//		.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
//		.CPUAccessFlags = 0,
//		.MiscFlags = 0,
//	};
//
//	ID3D11Texture2D* backBuffer;
//	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
//	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &m_backBufferRTV));
//	backBuffer->Release();
//
//	// Check multi sampling
//	UINT _4xMsaaQuality;
//	HR(m_d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality));
//
//	// create depth stencil view
//	D3D11_TEXTURE2D_DESC depthStencilDesc;
//	depthStencilDesc.Width = m_screenWidth;
//	depthStencilDesc.Height = m_screenHeight;
//	depthStencilDesc.MipLevels = 1;
//	depthStencilDesc.ArraySize = 1;
//	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//
//	if (_4xMsaaQuality > 0)
//	{
//		depthStencilDesc.SampleDesc.Count = 4;
//		depthStencilDesc.SampleDesc.Quality = _4xMsaaQuality - 1;
//	}
//	else
//	{
//		depthStencilDesc.SampleDesc.Count = 1;
//		depthStencilDesc.SampleDesc.Quality = 0;
//	}
//
//	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
//	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//	depthStencilDesc.CPUAccessFlags = 0;
//	depthStencilDesc.MiscFlags = 0;
//
//	HR(m_d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &m_depthStencilBuffer));
//	HR(m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, &m_depthStencilView));
//
//	// bind render target view and depth/stencil view
//	m_d3dDeviceContext->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), m_depthStencilView.Get());
//
//	// set the viewport transform
//	m_viewPort.TopLeftX = 0;
//	m_viewPort.TopLeftY = 0;
//	m_viewPort.Width = static_cast<float>(m_screenWidth);
//	m_viewPort.Height = static_cast<float>(m_screenHeight);
//	m_viewPort.MinDepth = 0.0f;
//	m_viewPort.MaxDepth = 1.0f;
//
//	m_d3dDeviceContext->RSSetViewports(1, &m_viewPort);
//}
//
//void DeferredRenderer::BuildQuadBuffers()
//{
//	// 정점 버퍼를 만든다.
//	VertexStruct::Vertex vertices[] =
//	{
//		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
//		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
//
//		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
//		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }
//	};
//
//	D3D11_BUFFER_DESC vbd;
//	vbd.Usage = D3D11_USAGE_IMMUTABLE;
//	vbd.ByteWidth = sizeof(VertexStruct::Vertex) * 4;
//	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vbd.CPUAccessFlags = 0;
//	vbd.MiscFlags = 0;
//	vbd.StructureByteStride = 0;
//	D3D11_SUBRESOURCE_DATA vinitData;
//	vinitData.pSysMem = vertices;
//	m_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_quadVB);
//
//	// 인덱스 버퍼를 만든다.
//	UINT indices[] = {
//		0,1,2,
//		0,2,3,
//	};
//
//	D3D11_BUFFER_DESC ibd;
//	ibd.Usage = D3D11_USAGE_IMMUTABLE;
//	ibd.ByteWidth = sizeof(UINT) * 6;
//	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	ibd.CPUAccessFlags = 0;
//	ibd.MiscFlags = 0;
//	ibd.StructureByteStride = 0;
//	D3D11_SUBRESOURCE_DATA iinitData;
//	iinitData.pSysMem = indices;
//	m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_quadIB);
//}
//
//void DeferredRenderer::SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
//{
//	m_d3dDeviceContext->OMSetRenderTargets(1, &rtv, dsv);
//}
//
//void DeferredRenderer::ClearRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, const float color[4])
//{
//	m_d3dDeviceContext->ClearRenderTargetView(rtv, color);
//	m_d3dDeviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//}
//
//void DeferredRenderer::EnableZBuffering()
//{
//	m_d3dDeviceContext->OMSetDepthStencilState(m_depthStencilStateEnable.Get(), 1);
//}
//
//void DeferredRenderer::DisableZBuffering()
//{
//	m_d3dDeviceContext->OMSetDepthStencilState(m_depthStencilStateDisable.Get(), 1);
//}
//
//void DeferredRenderer::SetCubemapDSS()
//{
//	m_d3dDeviceContext->OMSetDepthStencilState(m_depthStencilStateCubeMap.Get(), 1);
//}
//
//void DeferredRenderer::CreateDepthStecilStates()
//{
//	// Initialize the depth stencil states
//	D3D11_DEPTH_STENCIL_DESC enableDepthStencilDescription;
//	ZeroMemory(&enableDepthStencilDescription, sizeof(enableDepthStencilDescription));
//
//	enableDepthStencilDescription.DepthEnable = true;
//	enableDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	enableDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
//	enableDepthStencilDescription.StencilEnable = true;
//	enableDepthStencilDescription.StencilReadMask = 0xFF;
//	enableDepthStencilDescription.StencilWriteMask = 0xFF;
//	// Stencil operations if pixel is front-facing.
//	enableDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	enableDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//	enableDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	enableDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//	// Stencil operations if pixel is back-facing.
//	enableDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	enableDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
//	enableDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	enableDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//	// Create the depth stencil state for enabling Z buffering
//	m_d3dDevice->CreateDepthStencilState(&enableDepthStencilDescription, &m_depthStencilStateEnable);
//
//	// Initialize the depth stencil states
//	D3D11_DEPTH_STENCIL_DESC disableDepthStencilDescription;
//	ZeroMemory(&disableDepthStencilDescription, sizeof(disableDepthStencilDescription));
//
//	disableDepthStencilDescription.DepthEnable = false;
//	disableDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	disableDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
//	disableDepthStencilDescription.StencilEnable = true;
//	disableDepthStencilDescription.StencilReadMask = 0xFF;
//	disableDepthStencilDescription.StencilWriteMask = 0xFF;
//	// Stencil operations if pixel is front-facing.
//	disableDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	disableDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//	disableDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	disableDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//	// Stencil operations if pixel is back-facing.
//	disableDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	disableDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
//	disableDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	disableDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//	// Create the depth stencil state for disabling Z buffering
//	m_d3dDevice->CreateDepthStencilState(&disableDepthStencilDescription, &m_depthStencilStateDisable);
//
//	// Initialize the depth stencil states
//	D3D11_DEPTH_STENCIL_DESC cubemapDepthStencilDescription;
//	ZeroMemory(&cubemapDepthStencilDescription, sizeof(cubemapDepthStencilDescription));
//
//	cubemapDepthStencilDescription.DepthEnable = true;
//	cubemapDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	cubemapDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
//	cubemapDepthStencilDescription.StencilEnable = true;
//	cubemapDepthStencilDescription.StencilReadMask = 0xFF;
//	cubemapDepthStencilDescription.StencilWriteMask = 0xFF;
//	// Stencil operations if pixel is front-facing.
//	cubemapDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	cubemapDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//	cubemapDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	cubemapDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//	// Stencil operations if pixel is back-facing.
//	cubemapDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	cubemapDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
//	cubemapDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	cubemapDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//	// Create the depth stencil state for cube mapping
//	m_d3dDevice->CreateDepthStencilState(&cubemapDepthStencilDescription, &m_depthStencilStateCubeMap);
//}
//
//void DeferredRenderer::SetLights()
//{
//	DirectionalLight* dirLight = new DirectionalLight();
//	dirLight->Color = XMFLOAT4{ 0.5f, 0.5f, 0.5f, 1.0f };
//	dirLight->Direction = XMFLOAT3{ 10.0f, -10.0f, 0.0f };
//	ResourceManager::Instance.Get().GetPixelShader("PixelShader.cso")->SetDirectionalLight("dirLight", *dirLight);
//	ResourceManager::Instance.Get().GetPixelShader("SkeletonPixelShader.cso")->SetDirectionalLight("dirLight", *dirLight);
//
//	PointLight pointLight[4];
//	pointLight[0].Color = XMFLOAT4{ 0.3f, 0.0f, 0.0f, 1.0f };
//	pointLight[0].Position = XMFLOAT4{ 5.0f, 5.0f, 0.0f, 1.0f };
//	pointLight[1].Color = XMFLOAT4{ 0.0f, 0.3f, 0.0f, 1.0f };
//	pointLight[1].Position = XMFLOAT4{ 0.0f, 3.0f, -5.0f, 1.0f };
//	pointLight[2].Color = XMFLOAT4{ 0.5f, 0.3f, 0.0f, 1.0f };
//	pointLight[2].Position = XMFLOAT4{ -1.0f, 0.0f, 0.0f, 1.0f };
//	pointLight[3].Color = XMFLOAT4{ 0.2f, 0.2f, 0.2f, 1.0f };
//	pointLight[3].Position = XMFLOAT4{ 0.0f, 3.0f, -10.0f, 1.0f };
//	ResourceManager::Instance.Get().GetPixelShader("PixelShader.cso")->SetPointLight("pointLight", pointLight);
//	ResourceManager::Instance.Get().GetPixelShader("SkeletonPixelShader.cso")->SetPointLight("pointLight", pointLight);
//
//	SpotLight spotLight[2];
//	spotLight[0].Color = XMFLOAT4{ 0.1f, 0.1f, 0.1f, 1.0f };
//	spotLight[0].Direction = XMFLOAT3{ 10.0f, -3.0f, 0.0f };
//	spotLight[0].Position = XMFLOAT4{ 20.0f, 10.0f, 0.0f, 1.0f };
//	spotLight[0].SpotPower = 1.0f;
//	spotLight[1].Color = XMFLOAT4{ 0.1f, 0.1f, 0.1f, 1.0f };
//	spotLight[1].Direction = XMFLOAT3{ -10.0f, 0.0f, -5.0f };
//	spotLight[1].Position = XMFLOAT4{ 10.0f, 20.0f, 5.0f, 1.0f };
//	spotLight[1].SpotPower = 1.0f;
//	ResourceManager::Instance.Get().GetPixelShader("PixelShader.cso")->SetSpotLight("spotLight", spotLight);
//	ResourceManager::Instance.Get().GetPixelShader("SkeletonPixelShader.cso")->SetSpotLight("spotLight", spotLight);
//}
//
//HRESULT DeferredRenderer::GetAdapterInfo()
//{
//	IDXGIAdapter1* pAdapter;
//	IDXGIFactory1* pFactory = NULL;
//
//	HRESULT hr = S_OK;
//
//	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
//	{
//		return E_FAIL;
//	}
//
//	hr = pFactory->EnumAdapters1(0, &pAdapter);
//
//	if (hr != DXGI_ERROR_NOT_FOUND)
//	{
//		pAdapter->GetDesc1(&m_AdapterDesc1);
//	}
//
//	pAdapter->Release();
//	pFactory->Release();
//
//	return S_OK;
//}
