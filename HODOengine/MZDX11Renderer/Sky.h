#pragma once
#include "DX11Define.h"
#include "IMeshObject.h"
#include <vector>

class Mesh;
class Material;
class VertexShader;
class PixelShader;

class Sky 
{
public:
	Sky();
	~Sky();

	void Update(float deltaTime);
	void Render();

	void SetMesh(const std::string& fileName);
	void SetVertexShader(const std::string& fileName);
	void SetPixelShader(const std::string& fileName);

	void SetCubeMapTexture(const std::string& fileName);
	void SetSamplerState(ID3D11SamplerState* sampler);

	void SetActive(bool isActive) { m_isActive = isActive; };

private:
	std::vector<Mesh*> m_meshes;
	Material* m_material;
	bool m_isActive;
};

