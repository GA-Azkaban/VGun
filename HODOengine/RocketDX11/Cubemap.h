#pragma once
#include <d3d11.h>
#include <vector>
#include <string>
#include "../HODO3DGraphicsInterface/ICubeMap.h"

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;

	class Cubemap : public HDEngine::ICubeMap
	{
	public:
		Cubemap();
		~Cubemap();

		void Update(float deltaTime);
		void Render();

		virtual void LoadCubeMapTexture(const std::string& fileName) override;
		virtual void SetActive(bool isActive) override { m_isActive = isActive; }

		void LoadMesh(const std::string& meshName);
		void LoadVertexShader(const std::string& fileName);
		void LoadPixelShader(const std::string& fileName);

	private:
		std::vector<Mesh*> m_meshes;
		Material* m_material;
		bool m_isActive;
	};
}

