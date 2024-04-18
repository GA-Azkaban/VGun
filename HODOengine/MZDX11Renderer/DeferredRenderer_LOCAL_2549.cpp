#include "ResourceManager.h"
#include "DeferredRenderer.h"
#include "DeferredBuffers.h"
#include "Grid.h"
#include "Axis.h"
#include "Pig.h"
#include "Box.h"
#include "Fox.h"
#include "TextureBox.h"
#include "MZCamera.h"
#include "Effects.h"
#include "StaticMesh.h"
#include "DebugCube.h"
#include "UIImage.h"
#include "UIText.h"

#include "ASEParser.h"
#include "SkinnedMesh.h"

MZRenderer::LazyObjects<DeferredRenderer> DeferredRenderer::Instance;

DeferredRenderer::DeferredRenderer()
	: m_depthStencilStateEnable(0), m_depthStencilStateDisable(0),
	switcher(0)
{
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
}

DeferredRenderer::~DeferredRenderer()
{
	DeferredBuffers::Instance.Get().Finalize();

	ReleaseCOM(m_depthStencilStateDisable);
	ReleaseCOM(m_depthStencilStateEnable);
}


void DeferredRenderer::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, float screenWidth, float screenHeight)
{
	m_d3dDevice = pDevice;
	m_d3dImmediateContext = pDeviceContext;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	SetUpViewport();

	// 렌더 타겟뷰, 뎁스 스텐실뷰 등을 생성한다.
	CreateResources();

	DeferredBuffers::Instance.Get().Initialize(m_d3dDevice.Get(), m_screenWidth, m_screenHeight);

	// 그리드
	Grid* grid = new Grid(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), RasterizerState::WireframeRS.Get());

	// 축
	Axis* axis = new Axis(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), RasterizerState::WireframeRS.Get());

	//UIText* uiText = new UIText(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), RasterizerState::SolidRS.Get());

	ResourceManager::Instance.Get().LoadFile((LPSTR)"ASEFile/genji_blender.ASE");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"ASEFile/babypig_walk_6x.ASE");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/fox.fbx");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/static_fox.fbx");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/static_tower.fbx");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/static_hotel.fbx");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/static_box.fbx");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/filledCircle.fbx");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/frontRectangle.fbx");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/frontTriangle.fbx");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/genji_max.fbx");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/Rob02Trang.fbx");
	//ResourceManager::Instance.Get().LoadFile((LPSTR)"FBXFile/fps_modeling.fbx");
	
	ResourceManager::Instance.Get().LoadFile((LPSTR)"ASEFile/genji_max2.ASE");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"ASEFile/Rob02Trang.ASE");
	ResourceManager::Instance.Get().LoadFile((LPSTR)"ASEFile/Ground_Small_1.ASE");
	ResourceManager::Instance.Get().LoadFile((LPCSTR)"Textures/fox_reverse.dds");
	ResourceManager::Instance.Get().LoadFile((LPCSTR)"Textures/red.dds");
	ResourceManager::Instance.Get().LoadFile((LPCSTR)"Textures/000000002406_b_reverse.dds");
	ResourceManager::Instance.Get().LoadFile((LPCSTR)"Textures/000000002405_reverse.dds");
	//ResourceManager::Instance.Get().LoadFile((LPCSTR)"Textures/Rob02Blue_AlbedoTransparency_reverse.dds");
	//ResourceManager::Instance.Get().LoadFile((LPCSTR)"Textures/Rob02_Normal_reverse.dds");
	ResourceManager::Instance.Get().LoadFile((LPCSTR)"Textures/Ground_NM_reverse.dds");
	ResourceManager::Instance.Get().LoadFile((LPCSTR)"Textures/Ground_ALB_reverse.dds");

	//UIImage* uiImage = new UIImage(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), RasterizerState::SolidRS.Get());
	//uiImage->SetImage((LPCSTR)"Textures/fox_reverse.dds");
	//XMMATRIX worldTM = XMMatrixIdentity();
	//worldTM.r[0].m128_f32[0] = 0.5f;
	//worldTM.r[1].m128_f32[1] = 0.5f;
	//worldTM.r[2].m128_f32[2] = 0.5f;
	//uiImage->SetWorldTM(worldTM);
	
	// Mesh Objects 

	/*Pig* pig = new Pig(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), m_solidRS.Get());
	pig->SetScaleMatrix(0.05f);
	IRenderableObject::renderables.emplace_back(pig);

    Pig* pig2 = new Pig(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), m_solidRS.Get());
    pig2->SetTranslateMatrix(5.0f, 0.0f, 5.0f);
    pig2->SetScaleMatrix(0.07f);
    IRenderableObject::renderables.emplace_back(pig2);*/

	//StaticMesh* genji = new StaticMesh(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), RasterizerState::SolidRS.Get());
	//genji->LoadDiffuseTexture(L"Textures/000000002405_reverse.dds");
	//genji->LoadNormalTexture(L"Textures/000000002406_b_reverse.dds");
	//genji->LoadStaticMesh((LPSTR)"ASEFile/genji_blender.ASE");
	////genji->SetRotateMatrix(0.0f);
	//genji->SetName("genji1");
	//IRenderableObject::renderables.emplace(genji);

	//   StaticMesh* genji2 = new StaticMesh(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), m_solidRS.Get());
	   //genji2->LoadDiffuseTexture(L"Textures/000000002405_reverse.dds");
	   //genji2->LoadNormalTexture(L"Textures/000000002406_b_reverse.dds");
	   //genji2->LoadStaticMesh((LPSTR)"ASEFile/genji_blender.ASE");
	//   genji2->SetTranslateMatrix(10.0f, 0.0f, 0.0f);
	//   genji2->SetRotateMatrix(7.0f);
	//   genji2->SetName("genji2");
	//   IRenderableObject::renderables.emplace_back(genji2);

	   //StaticMesh* genji3 = new StaticMesh(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), m_solidRS.Get());
	   //genji3->LoadDiffuseTexture(L"Textures/000000002405_reverse.dds");
	   //genji3->LoadNormalTexture(L"Textures/000000002406_b_reverse.dds");
	   //genji3->LoadStaticMesh((LPSTR)"ASEFile/genji_blender.ASE");
	   //genji3->SetTranslateMatrix(20.0f, 0.0f, 0.0f);
	   //genji3->SetRotateMatrix(10.0f);
	   //genji3->SetName("genji3");
	   //IRenderableObject::renderables.emplace_back(genji3);

	   //StaticMesh* fox2 = new StaticMesh(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), m_solidRS.Get());
	   //fox2->LoadDiffuseTexture(L"Textures/fox_reverse.dds");
	   //fox2->LoadNormalTexture(L"Textures/000000002406_b.dds");
	   //fox2->LoadStaticMesh((LPSTR)"FBXFile/static_fox.fbx");
	   //fox2->SetTranslateMatrix(10.0f, 5.0f, 0.0f);
	   //fox2->SetScaleMatrix(0.1f);
	   //fox2->SetRotateMatrix(10.0f);
	   //IRenderableObject::renderables.emplace_back(fox2);

	   //Fox* fox = new Fox(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), m_solidRS.Get());
	   //fox->SetTranslateMatrix(10.0f, 0.0f, 5.0f);
	   //fox->SetScaleMatrix(0.01f);
	   //IRenderableObject::renderables.emplace_back(fox);

       //std::unique_ptr<ASEParser> m_pASEParser = make_unique<ASEParser>();
       //m_pASEParser->Init();
       ////m_pASEParser->Load((LPSTR)"ASEFile/babypig_idle_6x.ASE");

    //auto skinnedMesh = new SkinnedMesh(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), RasterizerState::SolidRS.Get());
    //skinnedMesh->LoadMesh("FBXFile/fox.fbx");
    //skinnedMesh->GetAnim().SetAnimation("FBXFile/fox.fbx-Take 001");
    //skinnedMesh->GetAnim().SetLoopMode(true);
    //skinnedMesh->GetAnim().Play();
    //skinnedMesh->materialPath = L"./Textures/";
    ////skinnedMesh->LoadGeometry(m_pASEParser->GetMeshes(), m_pASEParser->nodeIdxByBoneIdx);
    ////skinnedMesh->LoadAnimation(m_pASEParser->m_list_animation, m_pASEParser->m_scenedata);
    ////skinnedMesh->LoadDiffuseMap(m_pASEParser->m_list_materialdata[0]->m_map_diffuse->m_bitmap);
    //skinnedMesh->SetDiffuseMap("Textures/fox_reverse.dds");
    //skinnedMesh->SetRootTM(
    //    //XMMatrixScaling(0.1f, -0.1f, 0.1f) *
    //    XMMatrixRotationAxis({ 1,0,0 }, -1.57f) *
    //    //XMMatrixTranslation(0, 50, 50)*
    //    XMMatrixScaling(0.01f, 0.01f, 0.01f)
    //);
    //IRenderableObject::renderables.push_back(skinnedMesh);
	//skinnedMesh->SetActive(false);

	// Texture Boxes
	for (int i = 0; i < 5; ++i)
	{
		TextureBox* box = new TextureBox(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), RasterizerState::SolidRS.Get());
		m_textureBoxes.emplace_back(box);
	}

	BuildQuadBuffers();

	SetLights();

	// Set the depth stencil state
	EnableZBuffering();
}

void DeferredRenderer::Update(MZCamera* pCamera, float deltaTime)
{
	// On/Off Print Mode of Texture boxes
	if (GetAsyncKeyState('P') & 0x0001)
	{
		if (switcher == 1)
		{
			switcher = 0;
			for (auto& object : IDebugObject::debugObjects)
			{
				object->SetActive(false);
			}
		}
		else if (switcher == 0)
		{
			switcher = 1;
			for (auto& object : IDebugObject::debugObjects)
			{
				object->SetActive(true);
			}
		}
		
	}

	for (auto& object : IMeshObject::meshObjects)
	{		
		object->Update(pCamera, deltaTime);
	}
	for (auto& object : IDebugObject::debugObjects)
	{		
		object->Update(pCamera, deltaTime);
	}
	for (auto& object : IUIObject::uiObjects)
	{		
		object->Update(pCamera, deltaTime);
	}

	for (auto& textureBox : m_textureBoxes)
	{
		textureBox->Update(pCamera);
	}

	//XMStoreFloat4x4(&m_world, XMMatrixIdentity());
	//XMStoreFloat4x4(&m_view, pCamera->View());
	//XMStoreFloat4x4(&m_proj, pCamera->Proj());

	mEyePosW = XMFLOAT3(pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);
}

void DeferredRenderer::RenderToBackBuffer()
{
	EnableZBuffering();

	DeferredBuffers::Instance.Get().SetRenderTargets(m_d3dImmediateContext.Get());
	DeferredBuffers::Instance.Get().ClearRenderTargets(m_d3dImmediateContext.Get(), DirectX::Colors::Black);

	// 첫번째 Tech를 돌면서 Render Target들에 텍스처들을 저장한다.
	for (auto object : IMeshObject::meshObjects)
	{		
		object->RenderToTexture();
	}
	for (auto object : IDebugObject::debugObjects)
	{		
		object->RenderToTexture();
	}

	// 최종적으로 하나의 Render Target에 모두 합쳐서 그린다.

	SetRenderTargets();
	ClearRenderTargets(reinterpret_cast<const float*>(&DirectX::Colors::Black));
	DisableZBuffering();

	RenderAll();

	for (auto& object : IUIObject::uiObjects)
	{
		object->Render();
	}

	// 디버그용 텍스처 박스 출력
	for (int i = 0; i < 5; ++i)
	{
		m_textureBoxes[i]->SetTexture(DeferredBuffers::Instance.Get().GetShaderResourceView(i));
		if (switcher == 1)
			m_textureBoxes[i]->Render();
	}

	/*ID3D11ShaderResourceView* shaderResView = NULL;
	Effects::DeferredFX->SetShaderResource(shaderResView, 0);
	Effects::DeferredFX->SetShaderResource(shaderResView, 1);
	Effects::DeferredFX->SetShaderResource(shaderResView, 2);
	Effects::DeferredFX->SetShaderResource(shaderResView, 3);
	Effects::DeferredFX->SetShaderResource(shaderResView, 4);*/

	ID3D11ShaderResourceView* shaderResView = NULL;
	m_d3dImmediateContext->PSSetShaderResources(0, 1, &shaderResView);
	m_d3dImmediateContext->PSSetShaderResources(1, 1, &shaderResView);
	m_d3dImmediateContext->PSSetShaderResources(2, 1, &shaderResView);
	m_d3dImmediateContext->PSSetShaderResources(3, 1, &shaderResView);
	m_d3dImmediateContext->PSSetShaderResources(4, 1, &shaderResView);
}

void DeferredRenderer::RenderAll()
{
	// 입력 배치 객체 셋팅
	m_d3dImmediateContext->IASetInputLayout(InputLayouts::DeferredInputLayout);

	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 렌더 스테이트
	m_d3dImmediateContext->RSSetState(RasterizerState::SolidRS.Get());

	/// WVP TM등을 셋팅
	// Set constants
	//XMMATRIX view = XMLoadFloat4x4(&m_view);
	//XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	//XMMATRIX world = XMLoadFloat4x4(&m_world);
	//XMMATRIX worldViewProj = world * view * proj;

	// 월드의 역행렬
	//XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

	// Set per frame constants.
	Effects::PBRFX->SetDirLights(m_dirLights);
	Effects::PBRFX->SetPointLights(m_pointLights);
	Effects::PBRFX->SetSpotLights(m_spotLights);
	Effects::PBRFX->SetEyePosW(mEyePosW);
	//Effects::PBRFX->SetMaterial(m_material);

	// Figure out which technique to use.
	ID3DX11EffectTechnique* mTech = Effects::PBRFX->ComputeLightingTech;

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);
	UINT stride = sizeof(VertexStruct::PosTex);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, &m_QuadVB, &stride, &offset);
	m_d3dImmediateContext->IASetIndexBuffer(m_QuadIB, DXGI_FORMAT_R32_UINT, 0);
	Effects::PBRFX->SetShaderResource(DeferredBuffers::Instance.Get().GetShaderResourceView(0), 0);
	Effects::PBRFX->SetShaderResource(DeferredBuffers::Instance.Get().GetShaderResourceView(1), 1);
	Effects::PBRFX->SetShaderResource(DeferredBuffers::Instance.Get().GetShaderResourceView(2), 2);
	Effects::PBRFX->SetShaderResource(DeferredBuffers::Instance.Get().GetShaderResourceView(3), 3);
	Effects::PBRFX->SetShaderResource(DeferredBuffers::Instance.Get().GetShaderResourceView(4), 4);

	mTech->GetPassByIndex(0)->Apply(0, m_d3dImmediateContext.Get());
	m_d3dImmediateContext->DrawIndexed(6, 0, 0);

}

void DeferredRenderer::SetRenderTargets()
{
	// Sets the render targets in the array as the location where the shaders will write to
	m_d3dImmediateContext.Get()->OMSetRenderTargets(1, m_backBufferRTV[frontBufferIndex].GetAddressOf(), m_depthStencilView.Get());
}

void DeferredRenderer::ClearRenderTargets(const float color[4])
{
	// 렌더 타겟 클리어
	m_d3dImmediateContext.Get()->ClearRenderTargetView(m_backBufferRTV[frontBufferIndex].Get(), color);
	// 뎁스 스텐실 뷰 클리어
	//m_d3dImmediateContext.Get()->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_d3dImmediateContext.Get()->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void* DeferredRenderer::GetSharedBackBufferTextureHandle()
{
	return m_backBufferSharedHandles[backBufferIndex];
}
void DeferredRenderer::SwapBackBuffer()
{
	backBufferIndex = frontBufferIndex;
	++frontBufferIndex %= BACK_BUFFER_COUNT;
}

IRenderableObject* DeferredRenderer::Pick(MZCamera* pCamera, float normalizedX, float normalizedY)
{
	float _screenCoordX = normalizedX * m_screenWidth;
	float _screenCoordY = normalizedY * m_screenHeight;
	XMMATRIX P = pCamera->Proj();

	// Compute picking ray in view space.
	// 2장과 같은 이유로 P(0,0) -> P.r[0].m128_f32[0]
	// 2장과 같은 이유로 P(1,1) -> P.r[1].m128_f32[1]
	float vx = (+2.0f * _screenCoordX / m_screenWidth - 1.0f) / P.r[0].m128_f32[0];
	float vy = (-2.0f * _screenCoordY / m_screenHeight + 1.0f) / P.r[1].m128_f32[1];

	pickedObjects.clear();

	for (auto& object : IUIObject::uiObjects)
	{
		if (object->Pick(pCamera, normalizedX, normalizedY))
		{
			float depth = object->GetDepth();
			pickedObjects.insert(std::make_pair(depth, object));
		}
		if (!pickedObjects.empty())
		{
			return pickedObjects.begin()->second;
		}
	}

    for (auto& object : IMeshObject::meshObjects)
    {
		if (object->Pick(pCamera, vx, vy))
		{
			float depth = object->GetDepth();
			pickedObjects.insert(std::make_pair(depth, object));
		}
		if (!pickedObjects.empty())
		{
			return pickedObjects.begin()->second;
		}
	}
	
	return nullptr;
}

void DeferredRenderer::SetLights()
{
	/// Set Directional Lights
	m_dirLights[0].Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_dirLights[0].Diffuse = XMFLOAT4(30.0f, 30.0f, 30.0f, 1.0f);
	m_dirLights[0].Specular = XMFLOAT4(0.0f, 0.2f, 0.2f, 1.0f);
	m_dirLights[0].Direction = XMFLOAT3(0.70f, 0.707f, -0.707f);

	/*m_dirLights[1].Ambient = XMFLOAT4(0.0f, 0.1f, 0.1f, 0.1f);
	m_dirLights[1].Diffuse = XMFLOAT4(10.0f, 10.0f, 10.0f, 1.0f);
	m_dirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	m_dirLights[1].Direction = XMFLOAT3(-0.57735f, 0.57735f, 0.57735f);

	m_dirLights[2].Ambient = XMFLOAT4(0.2f, 0.1f, 0.1f, 1.0f);
	m_dirLights[2].Diffuse = XMFLOAT4(10.5f, 10.5f, 10.0f, 1.0f);
	m_dirLights[2].Specular = XMFLOAT4(0.2f, 0.2f, 0.3f, 0.1f);
	m_dirLights[2].Direction = XMFLOAT3(0.0f, 0.0f, -10.0f);*/

	/// Set Point Lights
	/*m_pointLights[0].Ambient = XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f);
	m_pointLights[0].Diffuse = XMFLOAT4(0.0f, 0.1f, 20.1f, 1.0f);
	m_pointLights[0].Specular = XMFLOAT4(1.0f, 0.1f, 0.1f, 1.0f);
	m_pointLights[0].Position = XMFLOAT3(10.0f, 0.0f, 0.0f);
	m_pointLights[0].Att = XMFLOAT3(10.0f, 10.0f, 10.0f);
	m_pointLights[0].Range = 10.0f;*/

	/*m_pointLights[1].Ambient = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	m_pointLights[1].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_pointLights[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_pointLights[1].Position = XMFLOAT3(5.0f, 5.0f, 5.0f);
	m_pointLights[1].Att = XMFLOAT3(10.0f, 10.0f, 10.0f);
	m_pointLights[1].Range = 20.0f;

	m_pointLights[2].Ambient = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	m_pointLights[2].Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pointLights[2].Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pointLights[2].Position = XMFLOAT3(10.0f, 10.0f, 10.0f);
	m_pointLights[2].Att = XMFLOAT3(10.0f, 10.0f, 10.0f);
	m_pointLights[2].Range = 5.0f;*/

	/// Set Spot Lights
	/*m_spotLights[0].Ambient = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_spotLights[0].Diffuse = XMFLOAT4(20.0f, 0.1f, 0.1f, 1.0f);
	m_spotLights[0].Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.3f);
	m_spotLights[0].Position = XMFLOAT3(10.0f, 20.0f, -2.0f);
	m_spotLights[0].Direction = XMFLOAT3(0.0f, 0.0f, 10.0f);
	m_spotLights[0].Att = XMFLOAT3(0.0f, 10.0f, 10.0f);
	m_spotLights[0].Range = 10.0f;
	m_spotLights[0].Spot = 2.0f;*/

	/*m_spotLights[1].Ambient = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	m_spotLights[1].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_spotLights[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_spotLights[1].Position = XMFLOAT3(5.0f, 5.0f, 5.0f);
	m_spotLights[1].Direction = XMFLOAT3(0.0f, 10.0f, 10.0f);
	m_spotLights[1].Att = XMFLOAT3(10.0f, 10.0f, 10.0f);
	m_spotLights[1].Range = 10.0f;
	m_spotLights[1].Spot = 10.0f;*/

	/*m_spotLights[2].Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_spotLights[2].Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_spotLights[2].Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_spotLights[2].Position = XMFLOAT3(10.0f, 10.0f, 10.0f);
	m_spotLights[2].Direction = XMFLOAT3(10.0f, 0.0f, 10.0f);
	m_spotLights[2].Att = XMFLOAT3(10.0f, 10.0f, 10.0f);
	m_spotLights[2].Range = 10.0f;
	m_spotLights[2].Spot = 10.0f;*/

	m_material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_material.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 16.0f);
}

void DeferredRenderer::BuildQuadBuffers()
{
	// 정점 버퍼를 만든다.
	VertexStruct::PosTex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexStruct::PosTex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_QuadVB));

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
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_QuadIB));
}

void DeferredRenderer::EnableZBuffering()
{
	m_d3dImmediateContext->OMSetDepthStencilState(m_depthStencilStateEnable.Get(), 1);
}

void DeferredRenderer::DisableZBuffering()
{
	m_d3dImmediateContext->OMSetDepthStencilState(m_depthStencilStateDisable.Get(), 1);
}

void DeferredRenderer::SetUpViewport()
{
	// set the viewport transform
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<float>(m_screenWidth);
	m_viewport.Height = static_cast<float>(m_screenHeight);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	// Set the viewport
	m_d3dImmediateContext.Get()->RSSetViewports(1, &m_viewport);
}

void DeferredRenderer::CreateResources()
{
	// Initialize the depth stencil states
	D3D11_DEPTH_STENCIL_DESC enableDepthStencilDescription;
	ZeroMemory(&enableDepthStencilDescription, sizeof(enableDepthStencilDescription));

	enableDepthStencilDescription.DepthEnable = true;
	enableDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	enableDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	enableDepthStencilDescription.StencilEnable = true;
	enableDepthStencilDescription.StencilReadMask = 0xFF;
	enableDepthStencilDescription.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	enableDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	enableDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	enableDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	enableDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	enableDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	enableDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	enableDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	enableDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state for enabling Z buffering
	HR(m_d3dDevice->CreateDepthStencilState(&enableDepthStencilDescription, &m_depthStencilStateEnable));

	D3D11_DEPTH_STENCIL_DESC disableDepthStencilDescription;
	ZeroMemory(&disableDepthStencilDescription, sizeof(disableDepthStencilDescription));

	disableDepthStencilDescription.DepthEnable = false;
	disableDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	disableDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	disableDepthStencilDescription.StencilEnable = true;
	disableDepthStencilDescription.StencilReadMask = 0xFF;
	disableDepthStencilDescription.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	disableDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	disableDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	disableDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	disableDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	disableDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	disableDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	disableDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	disableDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state for disabling Z buffering
	HR(m_d3dDevice->CreateDepthStencilState(&disableDepthStencilDescription, &m_depthStencilStateDisable));

	// 백버퍼 텍스처들을 필요한 만큼 생성한다.
	D3D11_TEXTURE2D_DESC bTextureDescription
	{
		.Width = (UINT)m_screenWidth,
		.Height = (UINT)m_screenHeight,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
		.SampleDesc{.Count = 1, .Quality = 0 },
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		.CPUAccessFlags = 0,
		.MiscFlags = D3D11_RESOURCE_MISC_SHARED,
	};

	for (unsigned int iTextureIndex = 0; iTextureIndex < BACK_BUFFER_COUNT; iTextureIndex++)
	{
		HR(m_d3dDevice->CreateTexture2D(&bTextureDescription, NULL, m_backBufferTextures[iTextureIndex].GetAddressOf()));
		IDXGIResource* dxgiResource{ nullptr };
		m_backBufferTextures[iTextureIndex]->QueryInterface<IDXGIResource>(&dxgiResource);
		dxgiResource->GetSharedHandle(&m_backBufferSharedHandles[iTextureIndex]);
		dxgiResource->Release();
	}

	// ----------------------------------------------------------------------------
	// 텍스처마다 렌더타겟 뷰를 생성한다.
	// 렌더타겟 뷰는 밉맵의 갯수를 받는다.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescription
	{
		.Format = bTextureDescription.Format,
		.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
		.Texture2D{.MipSlice = 0}
	};

	for (int iTextureIndex = 0; iTextureIndex < BACK_BUFFER_COUNT; iTextureIndex++)
		HR(m_d3dDevice->CreateRenderTargetView(m_backBufferTextures[iTextureIndex].Get(),
			&renderTargetViewDescription, m_backBufferRTV[iTextureIndex].GetAddressOf()));

	// 뎁스 스텐실 버퍼를 생성한다.
	D3D11_TEXTURE2D_DESC depthStencilDesc
	{
		.Width = (UINT)m_screenWidth,
		.Height = (UINT)m_screenHeight,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		.SampleDesc{.Count = 1, .Quality = 0},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_DEPTH_STENCIL,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
	};
	HR(m_d3dDevice->CreateTexture2D(&depthStencilDesc, NULL, &m_depthStencilBuffer));

	// 뎁스 스텐실 뷰를 생성한다.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc
	{
		.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D,
		.Flags = 0,
		.Texture2D{.MipSlice = 0},
	};
	HR(m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(),
		&depthStencilViewDesc,
		m_depthStencilView.GetAddressOf()));
}

