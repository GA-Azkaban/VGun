#pragma once
#include "../HODO3DGraphicsInterface/ICamera.h"
#include "MZCamera.h"
#include "MZDX11Renderer.h"
#include "HODOmath.h"

namespace hodoGIAdapter
{
	class CameraAdapter : public HDEngine::ICamera
	{
	public:
		MZCamera mzCamera;

		CameraAdapter() {
			mzCamera.SetFrustum(0.25f * MathHelper::Pi, MZDX11Renderer::Instance().GetAspectRatio(), 1.0f, 1000.0f);
		};
		virtual ~CameraAdapter() {};

		virtual void SetPositionAndRotation(const HDMath::HDFLOAT3& pos, const HDMath::HDQuaternion& rot)
		{
			XMFLOAT3 position;
			position.x = pos.x;
			position.y = pos.y;
			position.z = pos.z;
			mzCamera.SetPosition(position);

			XMFLOAT4 quaternion;
			quaternion.x = rot.x;
			quaternion.y = rot.y;
			quaternion.z = rot.z;
			quaternion.w = rot.w;
			mzCamera.SetRotation(quaternion);

			mzCamera.UpdateViewMatrix();
		}

		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& tm) {
			XMMATRIX xmTM;

			xmTM.r[0].m128_f32[0] = tm._11;
			xmTM.r[0].m128_f32[1] = tm._12;
			xmTM.r[0].m128_f32[2] = tm._13;
			xmTM.r[0].m128_f32[3] = tm._14;

			xmTM.r[1].m128_f32[0] = tm._21;
			xmTM.r[1].m128_f32[1] = tm._22;
			xmTM.r[1].m128_f32[2] = tm._23;
			xmTM.r[1].m128_f32[3] = tm._24;

			xmTM.r[2].m128_f32[0] = tm._31;
			xmTM.r[2].m128_f32[1] = tm._32;
			xmTM.r[2].m128_f32[2] = tm._33;
			xmTM.r[2].m128_f32[3] = tm._34;

			xmTM.r[3].m128_f32[0] = tm._41;
			xmTM.r[3].m128_f32[1] = tm._42;
			xmTM.r[3].m128_f32[2] = tm._43;
			xmTM.r[3].m128_f32[3] = tm._44;

			mzCamera.SetWorldTM(xmTM);
			//auto inverse = XMMatrixInverse(nullptr, reinterpret_cast<const XMMATRIX&>(tm));
			//mzCamera.SetViewMatrix(XMMatrixInverse(nullptr, reinterpret_cast<const XMMATRIX&>(tm)));
		};
		virtual void SetNearZ(float nearZ) {}
		virtual void SetFarZ(float farZ) {}
		virtual void SetAspect(float aspect) {}
		virtual void SetFOVY(float fov) {}
		virtual void SetNearHeight(float height) {}
		virtual void SetFarHeight(float height) {}
		virtual void SetAsMainCamera() { mzCamera.SetMain(); };
	};
}
