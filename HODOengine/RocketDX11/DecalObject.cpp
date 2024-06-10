#include "DecalObject.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace RocketCore::Graphics
{

	DecalObject::DecalObject()
		: _material(nullptr)
	{
		_mesh = ResourceManager::Instance().GetMeshes("primitiveDecalBox")[0];
		_boundingBox = ResourceManager::Instance().GetBoundingBox("primitiveDecalBox");
		_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
		_vertexShader = ResourceManager::Instance().GetVertexShader("DecalVertexShader.cso");
		_pixelShader = ResourceManager::Instance().GetPixelShader("DecalPixelShader.cso");
	}

	DecalObject::~DecalObject()
	{
		_mesh = nullptr;
		_material = nullptr;
		_vertexShader = nullptr;
		_pixelShader = nullptr;
		_rasterizerState.Reset();
	}

	void DecalObject::SetWorldTM(const Matrix& worldTM)
	{
		_world = worldTM;
		_receiveTMInfoFlag = true;
	}

	void DecalObject::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void DecalObject::LoadMaterial(HDEngine::IMaterial* material)
	{
		Material* newMat = dynamic_cast<Material*>(material);
		if (newMat != nullptr)
		{
			_material = newMat;
		}
	}

	void DecalObject::SetColor(UINT r, UINT g, UINT b, UINT a)
	{
		if (_material != nullptr)
		{
			_material->SetColor(r, g, b, a);
		}
	}

	void DecalObject::Render()
	{
		if (!_isActive)
			return;

		if (_receiveTMInfoFlag)
		{
			ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			ResourceManager::Instance().GetDeviceContext()->RSSetState(_rasterizerState.Get());

			XMMATRIX invWorld = XMMatrixInverse(nullptr, _world);

			_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(_world));
			_vertexShader->SetMatrix4x4("invWorld", (invWorld));

			_vertexShader->CopyAllBufferData();
			_vertexShader->SetShader();

			if (_material != nullptr)
			{
				_pixelShader->SetShaderResourceView("Albedo", _material->GetAlbedoMap());
				_pixelShader->SetFloat4("albedoColor", _material->GetColorFloat4());
			}

			_pixelShader->CopyAllBufferData();
			_pixelShader->SetShader();

			_mesh->BindBuffers();
			_mesh->Draw();
		}

		_receiveTMInfoFlag = false;
	}

	DirectX::BoundingBox DecalObject::GetBoundingBox()
	{
		BoundingBox bb = _boundingBox;
		bb.Transform(bb, _world);
		return bb;
	}

	bool DecalObject::IsCameraVisible()
	{
		return _cameraVisible;
	}

	void DecalObject::SetCameraVisible(bool isVisible)
	{
		_cameraVisible = isVisible;
	}

}
