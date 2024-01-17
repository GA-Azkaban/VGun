#pragma once
#include <wrl.h>
#include "IShader.h"
#include "GraphicsStruct.h"

namespace RocketCore::Graphics
{
	class VertexShader : public IShader
	{
	public:
		VertexShader();
		~VertexShader();

	public:
		virtual void Initialize(ID3D11Device* device, const std::string& path) override;

	public:
		ID3D11VertexShader* GetVertexShader() const;
		ID3D11Buffer* GetMatrixBuffer() const;
		ID3D11Buffer** GetAddressOfMatrixBuffer();
		ID3D11InputLayout* GetInputLayout() const;
		ID3D11SamplerState** GetAddressOfSampleState();

	public:
		void SetVertexDesc(D3D11_INPUT_ELEMENT_DESC* desc);
		void SetVertexType(VertexType type);
		VertexType GetVertexType() const;

	private:
		void CreateShaderAndInputLayout(ID3D11Device* device, const std::string& path);
		void CreateMatrixBuffer(ID3D11Device* device);
		void CreateSamplerState(ID3D11Device* device);

	private:
		D3D11_INPUT_ELEMENT_DESC* _vertexDesc;
		ComPtr<ID3D11VertexShader> _vertexShader;
		ComPtr<ID3D11Buffer> _matrixBuffer;
		ComPtr<ID3D11InputLayout> _inputLayout;
		ComPtr<ID3D11SamplerState> _sampleState;
		VertexType _vertexType;


		/// 2024.01.15 김민정
	public:
		VertexShader(ID3D11Device* device, ID3D11DeviceContext* context);
		//virtual ~VertexShader();
		ID3D11VertexShader* GetVertexShader() { return shader; }
		virtual void SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv) override;
		virtual void SetSamplerState(std::string name, ID3D11SamplerState* samplerState) override;

	protected:
		virtual bool CreateShader(ID3DBlob* shaderBlob) override;
		virtual void SetShaderAndConstantBuffers() override;
		virtual void CleanUp() override;

	private:
		ID3D11VertexShader* shader;
		ID3D11InputLayout* inputLayout;
	};
}


template <typename T>
ULONG GetRefCount(const ComPtr<T>& p)
{
	T* temp = p.Get();

	ULONG ret = 0;
	if (temp != nullptr)
	{
		ret = temp->AddRef();
		ret = temp->Release();
	}

	return ret;
}