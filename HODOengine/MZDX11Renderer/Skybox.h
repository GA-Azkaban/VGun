#pragma once
#include <vector>
#include <string>

class Mesh;
class Material;
class VertexShader;
class PixelShader;

class Skybox
{
public:
	friend class SkyboxPass;

	Skybox();
	~Skybox();

	void Update(float deltaTime);
	void Render();

	void SetCubeMapTexture(const std::string& fileName);
	void SetActive(bool isActive) { _isActive = isActive; };

	void SetMesh(const std::string& meshName);
	//void SetVertexShader(const std::string& fileName);
	//void SetPixelShader(const std::string& fileName);

private:
	static std::vector<Skybox*> _skyboxObjects;
	std::vector<Mesh*> _meshes;
	Material* _material;
	bool _isActive;
};

