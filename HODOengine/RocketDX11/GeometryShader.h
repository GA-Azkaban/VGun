#pragma once
#include <wrl.h>
#include "IShader.h"

namespace RocketCore::Graphics
{
	class GeometryShader : public IShader
	{
	public:
		GeometryShader(ID3D11Device* device, ID3D11DeviceContext* context, bool useStreamOut = false, bool allowStreamOutRasterization = false);
		virtual ~GeometryShader();

		ID3D11GeometryShader* GetGeometryShader();

		virtual void SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv) override;
		virtual void SetSamplerState(std::string name, ID3D11SamplerState* samplerState) override;

		bool CreateCompatibleStreamOutBuffer(ID3D11Buffer** buffer, int vertexCount);

		static void UnbindStreamOutStage(ID3D11DeviceContext* deviceContext);

	protected:
		virtual bool CreateShader(ID3DBlob* shaderBlob) override;
		virtual void SetShaderAndConstantBuffers() override;
		virtual void CleanUp() override;

		bool CreateShaderWithStreamOut(ID3DBlob* shaderBlob);

		// Helpers
		unsigned int CalcComponentCount(unsigned int mask);

	protected:
		ID3D11GeometryShader* _shader;
		bool _useStreamOut;
		bool _allowStreamOutRasterization;
		unsigned int _streamOutVertexSize;
	};
}
