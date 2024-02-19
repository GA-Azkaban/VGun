#include "GBufferPass.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "DeferredBuffers.h"

namespace RocketCore::Graphics
{
	GBufferPass::GBufferPass(DeferredBuffers* deferredBuffers)
		: _deferredBuffers(deferredBuffers), _deviceContext(ResourceManager::Instance().GetDeviceContext())
	{

	}

	GBufferPass::~GBufferPass()
	{
		delete _deferredBuffers;
	}

	void GBufferPass::Render()
	{
		_deferredBuffers->SetRenderTargets();
		_deferredBuffers->ClearRenderTargets();

		for (auto staticMeshObj : ObjectManager::Instance().GetStaticMeshObjList())
		{
			staticMeshObj->Render();
		}

		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			skinningMeshObj->Render();
		}

		ID3D11ShaderResourceView* nullSRV = nullptr;
		_deviceContext->PSSetShaderResources(0, 1, &nullSRV);
		_deviceContext->PSSetShaderResources(1, 1, &nullSRV);
		_deviceContext->PSSetShaderResources(2, 1, &nullSRV);
	}
}
