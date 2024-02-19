//***************************************************************************************
// GeometryGenerator.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "GeometryGenerator.h"
#include "MathHelper.h"
#include <DirectXColors.h>

namespace RocketCore::Graphics
{
	void GeometryGenerator::CreateGrid(DebugMeshData& meshData)
	{
		// 정점 버퍼를 만든다.
		PosColor vertices[100];
		for (int i = 0; i < 100; ++i)
		{
			vertices[i].Position = XMFLOAT3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
			vertices[i].Color = XMFLOAT4((const float*)&Colors::DarkGray);
		}

		meshData.Vertices.assign(&vertices[0], &vertices[100]);

		// 인덱스 버퍼를 만든다.
		UINT indices[40];
		for (int i = 0; i < 10; i++)
		{
			indices[i * 2] = i;
			indices[i * 2 + 1] = i + 90;
		}

		for (int i = 0; i < 10; i++)
		{
			indices[20 + (i * 2)] = i * 10;
			indices[20 + (i * 2) + 1] = i * 10 + 9;
		}

		meshData.Indices.assign(&indices[0], &indices[40]);
	}

	void GeometryGenerator::CreateAxis(DebugMeshData& meshData)
	{
		// 정점 버퍼를 만든다.
		PosColor vertices[] =
		{
			{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },	// x축 (빨강)
			{ XMFLOAT3(10.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },

			{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},	// y축 (초록)
			{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},

			{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)	},	// z축 (파랑)
			{ XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT4((const float*)&Colors::Blue) }
		};

		meshData.Vertices.assign(&vertices[0], &vertices[6]);

		// 인덱스 버퍼를 만든다.
		UINT indices[] = {
			// x축
			0, 1,

			// y축
			2, 3,

			// z축
			4, 5,
		};

		meshData.Indices.assign(&indices[0], &indices[6]);
	}

	void GeometryGenerator::CreateSkybox(MeshData& meshData)
	{
		Vertex vertices[] =
		{
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) }
		};

		meshData.Vertices.assign(&vertices[0], &vertices[24]);

		UINT indices[] =
		{
			0,  1,  2,  0,  2,  3,
			4,  5,  6,  4,  6,  7,
			8,  9, 10,  8, 10, 11,
		   12, 13, 14, 12, 14, 15,
		   16, 17, 18, 16, 18, 19,
		   20, 21, 22, 20, 22, 23
		};

		meshData.Indices.assign(&indices[0], &indices[36]);
	}

	void GeometryGenerator::CreateBox(float width, float height, float depth, MeshData& meshData)
	{
		//
		// Create the vertices.
		//

		Vertex v[24];

		float w2 = 0.5f * width;
		float h2 = 0.5f * height;
		float d2 = 0.5f * depth;

		// Fill in the front face vertex data.
		v[0] = Vertex(-w2, -h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
		v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
		v[2] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
		v[3] = Vertex(+w2, -h2, -d2, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);

		// Fill in the back face vertex data.
		v[4] = Vertex(-w2, -h2, +d2, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
		v[5] = Vertex(+w2, -h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
		v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
		v[7] = Vertex(-w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);

		// Fill in the top face vertex data.
		v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		v[9] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		v[10] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		v[11] = Vertex(+w2, +h2, -d2, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);

		// Fill in the bottom face vertex data.
		v[12] = Vertex(-w2, -h2, -d2, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[13] = Vertex(+w2, -h2, -d2, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[14] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[15] = Vertex(-w2, -h2, +d2, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);

		// Fill in the left face vertex data.
		v[16] = Vertex(-w2, -h2, +d2, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		v[17] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		v[18] = Vertex(-w2, +h2, -d2, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		v[19] = Vertex(-w2, -h2, -d2, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);

		// Fill in the right face vertex data.
		v[20] = Vertex(+w2, -h2, -d2, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[21] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[23] = Vertex(+w2, -h2, +d2, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

		meshData.Vertices.assign(&v[0], &v[24]);

		//
		// Create the indices.
		//

		UINT i[36];

		// Fill in the front face index data
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		// Fill in the back face index data
		i[6] = 4; i[7] = 5; i[8] = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		// Fill in the top face index data
		i[12] = 8; i[13] = 9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		// Fill in the bottom face index data
		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		// Fill in the left face index data
		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		// Fill in the right face index data
		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		meshData.Indices.assign(&i[0], &i[36]);
	}

	void GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData)
	{
		meshData.Vertices.clear();
		meshData.Indices.clear();

		//
		// Compute the vertices stating at the top pole and moving down the stacks.
		//

		// Poles: note that there will be texture coordinate distortion as there is
		// not a unique point on the texture map to assign to the pole when mapping
		// a rectangular texture onto a sphere.
		Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f);

		meshData.Vertices.push_back(topVertex);

		float phiStep = XM_PI / stackCount;
		float thetaStep = 2.0f * XM_PI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (UINT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vertex v;

				// spherical to cartesian
				v.Position.x = radius * sinf(phi) * cosf(theta);
				v.Position.y = radius * cosf(phi);
				v.Position.z = radius * sinf(phi) * sinf(theta);

				// Partial derivative of P with respect to theta
				v.Tangent.x = -radius * sinf(phi) * sinf(theta);
				v.Tangent.y = 0.0f;
				v.Tangent.z = +radius * sinf(phi) * cosf(theta);

				XMVECTOR T = XMLoadFloat3(&v.Tangent);
				XMStoreFloat3(&v.Tangent, XMVector3Normalize(T));

				XMVECTOR p = XMLoadFloat3(&v.Position);
				XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

				v.UV.x = theta / XM_2PI;
				v.UV.y = phi / XM_PI;

				meshData.Vertices.push_back(v);
			}
		}

		meshData.Vertices.push_back(bottomVertex);

		//
		// Compute indices for top stack.  The top stack was written first to the vertex buffer
		// and connects the top pole to the first ring.
		//

		for (UINT i = 1; i <= sliceCount; ++i)
		{
			meshData.Indices.push_back(0);
			meshData.Indices.push_back(i + 1);
			meshData.Indices.push_back(i);
		}

		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		UINT baseIndex = 1;
		UINT ringVertexCount = sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
				meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole vertex was added last.
		UINT southPoleIndex = (UINT)meshData.Vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(southPoleIndex);
			meshData.Indices.push_back(baseIndex + i);
			meshData.Indices.push_back(baseIndex + i + 1);
		}
	}

	void GeometryGenerator::Subdivide(MeshData& meshData)
	{
		// Save a copy of the input geometry.
		MeshData inputCopy = meshData;


		meshData.Vertices.resize(0);
		meshData.Indices.resize(0);

		//       v1
		//       *
		//      / \
		//     /   \
		//  m0*-----*m1
		//   / \   / \
		//  /   \ /   \
		// *-----*-----*
		// v0    m2     v2

		UINT numTris = inputCopy.Indices.size() / 3;
		for (UINT i = 0; i < numTris; ++i)
		{
			Vertex v0 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 0]];
			Vertex v1 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 1]];
			Vertex v2 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 2]];

			//
			// Generate the midpoints.
			//

			Vertex m0, m1, m2;

			// For subdivision, we just care about the position component.  We derive the other
			// vertex components in CreateGeosphere.

			m0.Position = XMFLOAT3(
				0.5f * (v0.Position.x + v1.Position.x),
				0.5f * (v0.Position.y + v1.Position.y),
				0.5f * (v0.Position.z + v1.Position.z));

			m1.Position = XMFLOAT3(
				0.5f * (v1.Position.x + v2.Position.x),
				0.5f * (v1.Position.y + v2.Position.y),
				0.5f * (v1.Position.z + v2.Position.z));

			m2.Position = XMFLOAT3(
				0.5f * (v0.Position.x + v2.Position.x),
				0.5f * (v0.Position.y + v2.Position.y),
				0.5f * (v0.Position.z + v2.Position.z));

			//
			// Add new geometry.
			//

			meshData.Vertices.push_back(v0); // 0
			meshData.Vertices.push_back(v1); // 1
			meshData.Vertices.push_back(v2); // 2
			meshData.Vertices.push_back(m0); // 3
			meshData.Vertices.push_back(m1); // 4
			meshData.Vertices.push_back(m2); // 5

			meshData.Indices.push_back(i * 6 + 0);
			meshData.Indices.push_back(i * 6 + 3);
			meshData.Indices.push_back(i * 6 + 5);

			meshData.Indices.push_back(i * 6 + 3);
			meshData.Indices.push_back(i * 6 + 4);
			meshData.Indices.push_back(i * 6 + 5);

			meshData.Indices.push_back(i * 6 + 5);
			meshData.Indices.push_back(i * 6 + 4);
			meshData.Indices.push_back(i * 6 + 2);

			meshData.Indices.push_back(i * 6 + 3);
			meshData.Indices.push_back(i * 6 + 1);
			meshData.Indices.push_back(i * 6 + 4);
		}
	}

	void GeometryGenerator::CreateGeosphere(float radius, UINT numSubdivisions, MeshData& meshData)
	{
		// Put a cap on the number of subdivisions.
		numSubdivisions = MathHelper::Min(numSubdivisions, 5u);

		// Approximate a sphere by tessellating an icosahedron.

		const float X = 0.525731f;
		const float Z = 0.850651f;

		XMFLOAT3 pos[12] =
		{
			XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),
			XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
			XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X),
			XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),
			XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f),
			XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
		};

		DWORD k[60] =
		{
			1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
			1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
			3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
			10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
		};

		meshData.Vertices.resize(12);
		meshData.Indices.resize(60);

		for (UINT i = 0; i < 12; ++i)
			meshData.Vertices[i].Position = pos[i];

		for (UINT i = 0; i < 60; ++i)
			meshData.Indices[i] = k[i];

		for (UINT i = 0; i < numSubdivisions; ++i)
			Subdivide(meshData);

		// Project vertices onto sphere and scale.
		for (UINT i = 0; i < meshData.Vertices.size(); ++i)
		{
			// Project onto unit sphere.
			XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.Vertices[i].Position));

			// Project onto sphere.
			XMVECTOR p = radius * n;

			XMStoreFloat3(&meshData.Vertices[i].Position, p);
			XMStoreFloat3(&meshData.Vertices[i].Normal, n);

			// Derive texture coordinates from spherical coordinates.
			float theta = MathHelper::AngleFromXY(
				meshData.Vertices[i].Position.x,
				meshData.Vertices[i].Position.z);

			float phi = acosf(meshData.Vertices[i].Position.y / radius);

			meshData.Vertices[i].UV.x = theta / XM_2PI;
			meshData.Vertices[i].UV.y = phi / XM_PI;

			// Partial derivative of P with respect to theta
			meshData.Vertices[i].Tangent.x = -radius * sinf(phi) * sinf(theta);
			meshData.Vertices[i].Tangent.y = 0.0f;
			meshData.Vertices[i].Tangent.z = +radius * sinf(phi) * cosf(theta);

			XMVECTOR T = XMLoadFloat3(&meshData.Vertices[i].Tangent);
			XMStoreFloat3(&meshData.Vertices[i].Tangent, XMVector3Normalize(T));
		}
	}

	void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData)
	{
		meshData.Vertices.clear();
		meshData.Indices.clear();

		//
		// Build Stacks.
		// 

		float stackHeight = height / stackCount;

		// Amount to increment radius as we move up each stack level from bottom to top.
		float radiusStep = (topRadius - bottomRadius) / stackCount;

		UINT ringCount = stackCount + 1;

		// Compute vertices for each stack ring starting at the bottom and moving up.
		for (UINT i = 0; i < ringCount; ++i)
		{
			float y = -0.5f * height + i * stackHeight;
			float r = bottomRadius + i * radiusStep;

			// vertices of ring
			float dTheta = 2.0f * XM_PI / sliceCount;
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				Vertex vertex;

				float c = cosf(j * dTheta);
				float s = sinf(j * dTheta);

				vertex.Position = XMFLOAT3(r * c, y, r * s);

				vertex.UV.x = (float)j / sliceCount;
				vertex.UV.y = 1.0f - (float)i / stackCount;

				// Cylinder can be parameterized as follows, where we introduce v
				// parameter that goes in the same direction as the v tex-coord
				// so that the bitangent goes in the same direction as the v tex-coord.
				//   Let r0 be the bottom radius and let r1 be the top radius.
				//   y(v) = h - hv for v in [0,1].
				//   r(v) = r1 + (r0-r1)v
				//
				//   x(t, v) = r(v)*cos(t)
				//   y(t, v) = h - hv
				//   z(t, v) = r(v)*sin(t)
				// 
				//  dx/dt = -r(v)*sin(t)
				//  dy/dt = 0
				//  dz/dt = +r(v)*cos(t)
				//
				//  dx/dv = (r0-r1)*cos(t)
				//  dy/dv = -h
				//  dz/dv = (r0-r1)*sin(t)

				// This is unit length.
				vertex.Tangent = XMFLOAT3(-s, 0.0f, c);

				float dr = bottomRadius - topRadius;
				XMFLOAT3 bitangent(dr * c, -height, dr * s);

				XMVECTOR T = XMLoadFloat3(&vertex.Tangent);
				XMVECTOR B = XMLoadFloat3(&bitangent);
				XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
				XMStoreFloat3(&vertex.Normal, N);

				meshData.Vertices.push_back(vertex);
			}
		}

		// Add one because we duplicate the first and last vertex per ring
		// since the texture coordinates are different.
		UINT ringVertexCount = sliceCount + 1;

		// Compute indices for each stack.
		for (UINT i = 0; i < stackCount; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				meshData.Indices.push_back(i * ringVertexCount + j);
				meshData.Indices.push_back((i + 1) * ringVertexCount + j);
				meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);

				meshData.Indices.push_back(i * ringVertexCount + j);
				meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);
				meshData.Indices.push_back(i * ringVertexCount + j + 1);
			}
		}

		BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
		BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	}

	void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
		UINT sliceCount, UINT stackCount, MeshData& meshData)
	{
		UINT baseIndex = (UINT)meshData.Vertices.size();

		float y = 0.5f * height;
		float dTheta = 2.0f * XM_PI / sliceCount;

		// Duplicate cap ring vertices because the texture coordinates and normals differ.
		for (UINT i = 0; i <= sliceCount; ++i)
		{
			float x = topRadius * cosf(i * dTheta);
			float z = topRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			meshData.Vertices.push_back(Vertex(x, y, z, u, v, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f));
		}

		// Cap center vertex.
		meshData.Vertices.push_back(Vertex(0.0f, y, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f));

		// Index of center vertex.
		UINT centerIndex = (UINT)meshData.Vertices.size() - 1;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(centerIndex);
			meshData.Indices.push_back(baseIndex + i + 1);
			meshData.Indices.push_back(baseIndex + i);
		}
	}

	void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
		UINT sliceCount, UINT stackCount, MeshData& meshData)
	{
		// 
		// Build bottom cap.
		//

		UINT baseIndex = (UINT)meshData.Vertices.size();
		float y = -0.5f * height;

		// vertices of ring
		float dTheta = 2.0f * XM_PI / sliceCount;
		for (UINT i = 0; i <= sliceCount; ++i)
		{
			float x = bottomRadius * cosf(i * dTheta);
			float z = bottomRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			meshData.Vertices.push_back(Vertex(x, y, z, u, v, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f));
		}

		// Cap center vertex.
		meshData.Vertices.push_back(Vertex(0.0f, y, 0.0f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f));

		// Cache the index of center vertex.
		UINT centerIndex = (UINT)meshData.Vertices.size() - 1;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(centerIndex);
			meshData.Indices.push_back(baseIndex + i);
			meshData.Indices.push_back(baseIndex + i + 1);
		}
	}

	void GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData)
	{
		UINT vertexCount = m * n;
		UINT faceCount = (m - 1) * (n - 1) * 2;

		//
		// Create the vertices.
		//

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		float dx = width / (n - 1);
		float dz = depth / (m - 1);

		float du = 1.0f / (n - 1);
		float dv = 1.0f / (m - 1);

		meshData.Vertices.resize(vertexCount);
		for (UINT i = 0; i < m; ++i)
		{
			float z = halfDepth - i * dz;
			for (UINT j = 0; j < n; ++j)
			{
				float x = -halfWidth + j * dx;

				meshData.Vertices[i * n + j].Position = XMFLOAT3(x, 0.0f, z);
				meshData.Vertices[i * n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				meshData.Vertices[i * n + j].Tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);

				// Stretch texture over grid.
				meshData.Vertices[i * n + j].UV.x = j * du;
				meshData.Vertices[i * n + j].UV.y = i * dv;
			}
		}

		//
		// Create the indices.
		//

		meshData.Indices.resize(faceCount * 3); // 3 indices per face

		// Iterate over each quad and compute indices.
		UINT k = 0;
		for (UINT i = 0; i < m - 1; ++i)
		{
			for (UINT j = 0; j < n - 1; ++j)
			{
				meshData.Indices[k] = i * n + j;
				meshData.Indices[k + 1] = i * n + j + 1;
				meshData.Indices[k + 2] = (i + 1) * n + j;

				meshData.Indices[k + 3] = (i + 1) * n + j;
				meshData.Indices[k + 4] = i * n + j + 1;
				meshData.Indices[k + 5] = (i + 1) * n + j + 1;

				k += 6; // next quad
			}
		}
	}

	void GeometryGenerator::CreateFullscreenQuad(MeshData& meshData)
	{
		meshData.Vertices.resize(4);
		meshData.Indices.resize(6);

		// Position coordinates specified in NDC space.
		meshData.Vertices[0] = Vertex(
			-1.0f, -1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f);

		meshData.Vertices[1] = Vertex(
			-1.0f, +1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f);

		meshData.Vertices[2] = Vertex(
			+1.0f, +1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f);

		meshData.Vertices[3] = Vertex(
			+1.0f, -1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f);

		meshData.Indices[0] = 0;
		meshData.Indices[1] = 1;
		meshData.Indices[2] = 2;

		meshData.Indices[3] = 0;
		meshData.Indices[4] = 2;
		meshData.Indices[5] = 3;
	}
}
