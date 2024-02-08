#pragma once
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <unordered_map>
#include "ShaderStruct.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class IShader
	{
	public:
		IShader(ID3D11Device* device, ID3D11DeviceContext* context);
		virtual ~IShader();

		// Initialize method
		bool LoadShaderFile(LPCWSTR shaderFile);

		// Activating the shader and copying data
		void SetShader();
		void CopyAllBufferData();
		void CopyBufferData(unsigned int index);
		void CopyBufferData(std::string bufferName);

		// Sets arbitrary shader data
		void SetData(std::string name, const void* data, unsigned int size);

		void SetInt(std::string name, int data);
		void SetFloat(std::string name, float data);
		void SetFloat2(std::string name, const float data[2]);
		void SetFloat2(std::string name, const DirectX::XMFLOAT2& data);
		void SetFloat3(std::string name, const float data[3]);
		void SetFloat3(std::string name, const DirectX::XMFLOAT3& data);
		void SetFloat4(std::string name, const float data[4]);
		void SetFloat4(std::string name, const DirectX::XMFLOAT4& data);
		void SetMatrix4x4(std::string name, const float data[16]);
		void SetMatrix4x4(std::string name, const DirectX::XMFLOAT4X4& data);
		void SetMatrix4x4(std::string name, const DirectX::XMMATRIX& data);
		void SetMatrix4x4Array(std::string name, const DirectX::XMMATRIX* data, UINT size = 1);

		virtual void SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv) = 0;
		virtual void SetSamplerState(std::string name, ID3D11SamplerState* samplerState) = 0;

		const SRVInfo* GetShaderResourceViewInfo(std::string name);
		const SRVInfo* GetShaderResourceViewInfo(unsigned int index);
		unsigned int GetShaderResourceViewCount() { return textureTable.size(); }

		const SamplerInfo* GetSamplerInfo(std::string name);
		const SamplerInfo* GetSamplerInfo(unsigned int index);
		unsigned int GetSamplerCount() { return samplerTable.size(); };

	protected:
		virtual bool CreateShader(ID3DBlob* shaderBlob) = 0;
		virtual void SetShaderAndConstantBuffers() = 0;

		virtual void CleanUp();

		ShaderVariableInfo* GetVariableInfo(std::string name, int size);
		ConstantBufferInfo* GetConstantBufferInfo(std::string name);

	protected:
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		ID3DBlob* shaderBlob;

		unsigned int constantBufferCount;

		ConstantBufferInfo* constantBuffers;
		std::vector<SRVInfo*> shaderResourceViews;
		std::vector<SamplerInfo*> samplerStates;
		std::unordered_map<std::string, ConstantBufferInfo*> constantBufferTable;
		std::unordered_map<std::string, ShaderVariableInfo> variableTable;
		std::unordered_map<std::string, SRVInfo*> textureTable;
		std::unordered_map<std::string, SamplerInfo*> samplerTable;
	};
}
