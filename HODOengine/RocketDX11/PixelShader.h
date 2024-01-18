#pragma once
#include <wrl.h>
#include "IShader.h"
#include "LightStruct.h"

namespace RocketCore::Graphics
{
	class PixelShader : public IShader
	{
	public:
		PixelShader();
		~PixelShader();

	public:
		virtual void Initialize(ID3D11Device* device, const std::string& path) override;

	public:
		ID3D11PixelShader* GetPixelShader() const;

	private:
		void CreatePixelShader(ID3D11Device* device, const std::string& path);


	private:
		ComPtr<ID3D11PixelShader> _pixelShader;



	/// 2024.01.15 김민정
	public:
		PixelShader(ID3D11Device* device, ID3D11DeviceContext* context);
		//~PixelShader();
		ID3D11PixelShader* GetPixelShader() { return shader; }

		virtual void SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv) override;
		virtual void SetSamplerState(std::string name, ID3D11SamplerState* samplerState) override;

		void SetDirectionalLight(std::string name, const DirectionalLight& dirLight);
		void SetPointLight(std::string name, const PointLight pointLight[4]);
		void SetSpotLight(std::string name, const SpotLight spotLight[2]);

	protected:
		virtual bool CreateShader(ID3DBlob* shaderBlob) override;
		virtual void SetShaderAndConstantBuffers() override;
		virtual void CleanUp() override;

	protected:
		ID3D11PixelShader* shader;
	};
}