#pragma once
#include "Component.h"
#include "dllExporter.h"
#include "..\\HODO3DGraphicsInterface\\ICubeMap.h"

namespace HDData
{
	class HODO_API CubeMapRenderer : public Component
	{
	public:
		CubeMapRenderer();

		void LoadCubeMapTexture(const std::string& fileName);

	protected:
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		HDEngine::ICubeMap* _cubeMap;
	};
}
