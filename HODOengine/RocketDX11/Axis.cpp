#include "Axis.h"
#include <DirectXColors.h>	// 이미 많은 부분 DX11과 겹친다.
#include <sstream>
#include <fstream>
#include <vector>
#include "RocketMacroDX11.h"

namespace RocketCore::Graphics
{
	Axis::Axis(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
		: _device(pDevice), _deviceContext(pDeviceContext), _renderstate(pRS),
		_vertexBuffer(nullptr), _indexBuffer(nullptr), _inputLayout(nullptr),
		_world(), _view(), _proj(),
		_vertexShader(nullptr), _pixelShader(nullptr)
	{

	}

	Axis::~Axis()
	{
		ReleaseCOM(_inputLayout);

		ReleaseCOM(_vertexBuffer);
		ReleaseCOM(_indexBuffer);
	}


	void Axis::Initialize()
	{
		BuildGeometryBuffers();
		CreateShader();
	}

	void Axis::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		_world = world;
		_view = view;
		_proj = proj;
	}

	void Axis::Render()
	{
		// Axis가 쓰는 Shader deviceContext 이용해 연결.
		_deviceContext->VSSetShader(_vertexShader, nullptr, 0);
		_deviceContext->PSSetShader(_pixelShader, nullptr, 0);

		// 입력 배치 객체 셋팅
		_deviceContext->IASetInputLayout(_inputLayout);
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBufferType* dataPtr;
		unsigned int bufferNumber;

		_world = DirectX::XMMatrixTranspose(_world);
		_view = DirectX::XMMatrixTranspose(_view);
		_proj = DirectX::XMMatrixTranspose(_proj);

		HR(_deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		dataPtr = (MatrixBufferType*)mappedResource.pData;

		dataPtr->world = _world;
		dataPtr->view = _view;
		dataPtr->projection = _proj;

		_deviceContext->Unmap(_matrixBuffer, 0);

		bufferNumber = 0;

		_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

		// 렌더스테이트
		_deviceContext->RSSetState(_renderstate);

		_deviceContext->DrawIndexed(6, 0, 0);
	}

	void Axis::BuildGeometryBuffers()
	{
		// 정점 버퍼를 생성한다. 
		// 각 축에 맞도록 6개의 정점을 만들었다.
		Vertex vertices[] =
		{
			{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Red)  },	// x축 (빨강)
			{ DirectX::XMFLOAT3(10.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Red)  },

			{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Green)},	// y축 (초록)
			{ DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Green)},

			{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Blue)	},	// z축 (파랑)
			{ DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Blue) }
		};

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * 6;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;
		HR(_device->CreateBuffer(&vbd, &vinitData, &_vertexBuffer));


		// 인덱스 버퍼를 생성한다.
		// 역시 3개의 축을 나타내도록 했다.

		UINT indices[] = {
			// x축
			0, 1,

			// y축
			2, 3,

			// z축
			4, 5,
		};

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * 6;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		HR(_device->CreateBuffer(&ibd, &iinitData, &_indexBuffer));
	}

	void Axis::CreateShader()
	{
		std::ifstream vsFile("../x64/Debug/VertexShader.cso", std::ios::binary);
		std::ifstream psFile("../x64/Debug/PixelShader.cso", std::ios::binary);

		std::vector<char> vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

		_device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &_vertexShader);
		_device->CreatePixelShader(psData.data(), psData.size(), nullptr, &_pixelShader);


		// 셰이더도 만들어두고 레이아웃도 만들어두고 이런거 저런거 갖다 쓸 수 있게하는게 좋겠지
		// 이렇게 코드적으로 박아두면 안좋을 것 같다는 얘기를 하는 것 같은데?

		// Create the vertex input layout.
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			//{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
			{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		HR(_device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer));

		_device->CreateInputLayout(vertexDesc, 2, vsData.data(), vsData.size(), &_inputLayout);
	}
}
