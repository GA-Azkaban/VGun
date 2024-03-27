#pragma once
#include <d3d11.h>
#include <vector>
#include <string>
#include "../HODO3DGraphicsInterface/ICubeMap.h"

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;
	class DeferredBuffers;

	class Cubemap : public HDEngine::ICubeMap
	{
	public:
		~Cubemap();
		static Cubemap& Instance();
		friend class SkyboxPass;

		void Update(float deltaTime);
		void Render();

		virtual void LoadCubeMapTexture(const std::string& fileName) override;
		virtual void SetActive(bool isActive) override { m_isActive = isActive; }
		virtual void SetEnvLightIntensity(float value) override { m_envLightIntensity = value; }

		void LoadMesh(const std::string& meshName);
		void LoadVertexShader(const std::string& fileName);
		void LoadPixelShader(const std::string& fileName);

		float GetEnvLightIntensity() { return m_envLightIntensity; }

	public:
		DeferredBuffers* _deferredBuffers;

	private:
		Cubemap();
		std::vector<Mesh*> m_meshes;
		Material* m_material;
		bool m_isActive;
		float m_size;
		float m_envLightIntensity;
	};
}

