#pragma once
#include "../HODO3DGraphicsInterface/IParticle.h"
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;

	class Particle : public HDEngine::IParticle
	{
	public:

	private:
		DirectX::XMMATRIX _world;
		Mesh* _mesh;
		Material* _material;
	};
}
