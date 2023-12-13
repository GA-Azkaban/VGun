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
		virtual void CreateShader(ID3D11Device* device, std::string path) override;

	public:
		ID3D11PixelShader* GetPixelShader() const;
		ID3D11Buffer* GetMatrixBuffer() const;
		ID3D11InputLayout* GetInputLayout() const;

	private:
		ComPtr<ID3D11PixelShader> _pixelShader;
		ComPtr<ID3D11Buffer> _matrixBuffer;
		ComPtr<ID3D11InputLayout> _inputLayout;
	};
}