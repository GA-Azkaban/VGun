#pragma once
#include <d3d11.h>
#include <vector>
#include <string>

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;

	class Cubemap
	{
	public:
		Cubemap();
		~Cubemap();

		void Update(float deltaTime);
		void Render();

		void LoadMesh(const std::string& meshName);
		void LoadVertexShader(const std::string& fileName);
		void LoadPixelShader(const std::string& fileName);
		void LoadCubeMapTexture(const std::string& fileName);

		void SetSamplerState(ID3D11SamplerState* sampler);

		void SetActive(bool isActive) { m_isActive = isActive; };

	private:
		std::vector<Mesh*> m_meshes;
		Material* m_material;
		bool m_isActive;
	};
}

