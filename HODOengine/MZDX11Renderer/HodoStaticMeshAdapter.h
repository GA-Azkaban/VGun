#pragma once
#include "../HODO3DGraphicsInterface/IStaticMesh.h"
#include "StaticMeshObject.h"
#include "MathHeader.h"
#include <memory>

namespace hodoGIAdapter
{
	class StaticMeshAdapter : public HDEngine::IStaticMesh
	{
	public:
		StaticMeshAdapter(StaticMeshObject* staticMeshObj)
			: _staticMeshObject(staticMeshObj)
		{

		}

		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;

		virtual void LoadMesh(const std::string& fileName) override;
		virtual void LoadNormalMap(const std::string& fileName) override;
		virtual void LoadDiffuseMap(const std::string& fileName) override;
		//virtual void LoadVertexShader(const std::string& fileName) override;
		//virtual void LoadPixelShader(const std::string& fileName) override;

	private:
		std::unique_ptr<StaticMeshObject> _staticMeshObject;
	};
}

