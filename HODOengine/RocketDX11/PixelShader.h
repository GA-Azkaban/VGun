#pragma once
#include <wrl.h>
#include "IShader.h"
#include "Light.h"

namespace RocketCore::Graphics
{
	class PixelShader : public IShader
	{
	public:
		PixelShader(ID3D11Device* device, ID3D11DeviceContext* context);
		virtual ~PixelShader();
		ID3D11PixelShader* GetPixelShader() { return shader; }

		virtual void SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv) override;
		virtual void SetSamplerState(std::string name, ID3D11SamplerState* samplerState) override;

		void SetLights(std::string name, const Light* lights);
		//void SetSpotLight(std::string name, const SpotLight spotLight[2]);

	protected:
		virtual bool CreateShader(ID3DBlob* shaderBlob) override;
		virtual void SetShaderAndConstantBuffers() override;
		virtual void CleanUp() override;

	protected:
		ID3D11PixelShader* shader;
	};
}
