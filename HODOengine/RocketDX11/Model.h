#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
#include <vector>

#include "IResource.h"
#include "GraphicsStruct.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Texture;
	class Mesh;

	class Model : public IResource
	{
	public:
		Model();
		Model(const Model& other);
		~Model();

		void Initialize(ID3D11Device* device);
		void Render(ID3D11DeviceContext* deviceContext);

		// 텍스쳐 로드, 반환
		bool LoadTexture(ID3D11Device* device, const std::wstring& filename);
		void ReleaseTexture();

	public:
		void AddMesh(Mesh* mesh);

	private:
		std::vector<Mesh*> _meshes;
		std::vector<Texture*> _textures;
	};
}
