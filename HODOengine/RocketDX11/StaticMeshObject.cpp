#include "StaticMeshObject.h"
#include "..\\HODOmath\\HODOmath.h"
#include "RocketMacroDX11.h"
#include "ResourceManager.h"
#include "GraphicsStruct.h"

namespace RocketCore::Graphics
{
	void StaticMeshObject::SetWorldTM(const HDMath::HDFLOAT4X4& worldTM)
	{
		DirectX::XMFLOAT4X4 temp;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				temp.m[i][j] = worldTM.element[i][j];
			}
		}

		_worldTM = XMLoadFloat4x4(&temp);
	}

	void StaticMeshObject::SetActive(bool isActive)
	{

	}

	void StaticMeshObject::SetVertexShader(VertexShader* shader)
	{
		_vertexShader = shader;
	}

	void StaticMeshObject::SetPixelShader(PixelShader* shader)
	{
		_pixelShader = shader;
	}

	void StaticMeshObject::LoadVertexShader(const std::string& fileName)
	{
	}

	void StaticMeshObject::LoadPixelShader(const std::string& fileName)
	{
	}

	void StaticMeshObject::LoadMesh(const std::string& fileName)
	{
	}

	void StaticMeshObject::LoadNormalMap(const std::string& fileName)
	{
	}

	void StaticMeshObject::LoadDiffuseMap(const std::string& fileName)
	{
	}

	void StaticMeshObject::Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		// Grid가 쓰는 Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(_vertexShader->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_pixelShader->GetPixelShader(), nullptr, 0);

		deviceContext->PSSetSamplers(0, 1, _vertexShader->GetAddressOfSampleState());

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(_vertexShader->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 상수 버퍼 세팅
		{
			// 버텍스 쉐이더
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(deviceContext->Map(_vertexShader->GetMatrixBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			MatrixBufferType* matrixBufferDataPtr = (MatrixBufferType*)mappedResource.pData;

			DirectX::XMMATRIX w = DirectX::XMMatrixTranspose(_worldTM);
			DirectX::XMMATRIX v = DirectX::XMMatrixTranspose(view);
			DirectX::XMMATRIX p = DirectX::XMMatrixTranspose(proj);

			matrixBufferDataPtr->world = w;
			matrixBufferDataPtr->view = v;
			matrixBufferDataPtr->projection = p;

			deviceContext->Unmap(_vertexShader->GetMatrixBuffer(), 0);

			unsigned int bufferNumber = 0;

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _vertexShader->GetAddressOfMatrixBuffer());

			// 픽셀 쉐이더
			HR(deviceContext->Map(_pixelShader->GetLightBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		
			LightBufferType* lightBufferDataPtr = (LightBufferType*)mappedResource.pData;

			lightBufferDataPtr->ambientColor = { 0.15f,0.15f,0.15f,1.0f };
			lightBufferDataPtr->diffuseColor = { 1.0f,1.0f,1.0f,1.0f };
			lightBufferDataPtr->lightDirection = { 1.0f,-1.0f,1.0f };
			lightBufferDataPtr->specularPower = 0.5f;
			lightBufferDataPtr->specularColor = { 1.0f,1.0f,1.0f,1.0f };

			deviceContext->Unmap(_pixelShader->GetLightBuffer(), 0);

			bufferNumber = 0;

			deviceContext->PSSetConstantBuffers(bufferNumber, 1, _pixelShader->GetAddressOfLightBuffer());
		}

		// 렌더스테이트
		deviceContext->RSSetState(_renderState);

		_model->Render(deviceContext);
	}

	StaticMeshObject::StaticMeshObject()
	{

	}

	void StaticMeshObject::SetModel(Model* model)
	{
		_model = model;
	}

}