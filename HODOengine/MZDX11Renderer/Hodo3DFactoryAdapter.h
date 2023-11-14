#pragma once
#include "IHodoGIFactory.h"
#include "Hodo3DCameraAdapter.h"

namespace hodoGI
{
	class ICamera;
}

namespace hodoGIAdapter
{
	class FactoryAdapter : public hodoGI::IFactory
	{
	public:
		virtual hodoGI::ICamera* CreateCamera() override{
			return new hodoGIAdapter::CameraAdapter();
		}
		/*virtual yunuGI::IStaticMesh* CreateStaticMesh(const yunuGI::IStaticMeshDesc& desc)
		{
			StaticMesh* staticMesh{ new StaticMesh(DeferredRenderer::Instance.Get().GetDevice(), DeferredRenderer::Instance.Get().GetDeviceContext(), RasterizerState::SolidRS.Get()) };
			return new StaticMeshAdapter{staticMesh};
		}
		virtual yunuGI::ISkinnedMesh* CreateSkinnedMesh(const yunuGI::ISkinnedMeshDesc& desc)
		{
			SkinnedMesh* skinnedMesh{ new SkinnedMesh(DeferredRenderer::Instance.Get().GetDevice(), DeferredRenderer::Instance.Get().GetDeviceContext(), RasterizerState::SolidRS.Get()) };
			return new SkinnedMeshAdapter{skinnedMesh};
		}
		virtual yunuGI::IDebugCube* CreateDebugCube(const yunuGI::IDebugCubeDesc& desc)
		{
			DebugCube* debugCube{ new DebugCube(DeferredRenderer::Instance.Get().GetDevice(), DeferredRenderer::Instance.Get().GetDeviceContext(), RasterizerState::WireframeRS.Get()) };
			return new DebugCubeAdapter{ debugCube };
		}
		virtual yunuGI::IUIImage* CreateUIImage(const yunuGI::IUIImageDesc& desc)
		{
			UIImage* uiImage{ new UIImage(DeferredRenderer::Instance.Get().GetDevice(), DeferredRenderer::Instance.Get().GetDeviceContext(), RasterizerState::SolidRS.Get()) };
			return new UIImageAdapter{ uiImage };
		}
		virtual yunuGI::IUIText* CreateUIText(const yunuGI::IUITextDesc& desc)
		{
			UIText* uiText{ new UIText(DeferredRenderer::Instance.Get().GetDevice(), DeferredRenderer::Instance.Get().GetDeviceContext(), RasterizerState::SolidRS.Get()) };
			return new UITextAdapter{ uiText };
		}
		virtual yunuGI::IDirectionalLight* CreateDirectionalLight(const yunuGI::IDirectionalLightDesc& desc) { return nullptr; }*/
	};
}