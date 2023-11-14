#pragma once
#include "IHodoGICamera.h"
#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "HODOmath.h"

namespace hodoGIAdapter
{
	class CameraAdapter : public hodoGI::ICamera
	{
	public:
		MZCamera mzCamera;

		CameraAdapter() {
			mzCamera.SetFrustum(0.25f * MathHelper::Pi, MZDX11Renderer::Instance.Get().GetAspectRatio(), 1.0f, 1000.0f);
		};
		virtual ~CameraAdapter() {};

		virtual void SetWorldTM(const HDMaths::HDFLOAT4X4& tm) {
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
			auto inverse = XMMatrixInverse(nullptr, reinterpret_cast<const XMMATRIX&>(tm));
			mzCamera.SetViewMatrix(XMMatrixInverse(nullptr, reinterpret_cast<const XMMATRIX&>(tm)));
		};
		virtual void SetFOV(float fov) {};
		virtual void SetNear(float near) {};
		virtual void SetFar(float far) {};
		virtual void SetAsMainCamera() { mzCamera.SetMain(); };
	};
}
