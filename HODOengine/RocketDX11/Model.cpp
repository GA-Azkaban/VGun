#include "Model.h"
#include "Texture.h"
#include "Mesh.h"
#include "GraphicsStruct.h"

namespace RocketCore::Graphics
{
	Model::Model()
		: _meshes(), _textures()
	{

	}

	Model::Model(const Model& other)
	{

	}

	Model::~Model()
	{

	}

	void Model::Initialize(ID3D11Device* device)
	{
		for (auto& mesh : _meshes)
		{
			mesh->Initialize(device);
		}
	}

	void Model::Render(ID3D11DeviceContext* deviceContext)
	{
		for (auto& mesh : _meshes)
		{
			// 인덱스버퍼와 버텍스버퍼 셋팅
			UINT stride = 0;
			UINT offset = 0;

			switch (mesh->GetVertexType())
			{
				case VertexType::COLOR_VERTEX:
					stride = sizeof(ColorVertex);
					break;
				case VertexType::TEXTURE_VERTEX:
					stride = sizeof(TextureVertex);
					break;
				case VertexType::LIGHT_VERTEX:
					stride = sizeof(LightVertex);
					break;
				default:
					break;
			}

			deviceContext->IASetVertexBuffers(0, 1, mesh->GetAddressOfVertexBuffer(), &stride, &offset);
			deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			deviceContext->PSSetShaderResources(0, 1, _textures[0]->GetAddressOfTextureView());

			deviceContext->DrawIndexed(mesh->GetIndexCount(),0,0);
		}
	}

	bool Model::LoadTexture(ID3D11Device* device, const std::wstring& filename)
	{
		ReleaseTexture();

		_textures.emplace_back(new Texture());

		// TODO : 임시로 하나만 쓰도록 함
		return _textures[0]->Initialize(device, filename);
	}

	void Model::ReleaseTexture()
	{
		for (auto& texture : _textures)
		{
			texture->Shutdown();
			delete texture;
		}

		_textures.clear();
	}

	void Model::AddMesh(Mesh* mesh)
	{
		_meshes.emplace_back(mesh);
	}

}
