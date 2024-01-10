#pragma once
#include <wrl.h>
#include "IShader.h"

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
	};
}