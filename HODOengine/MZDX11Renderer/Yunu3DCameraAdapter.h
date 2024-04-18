#pragma once
#include "IYunuGICamera.h"
#include "MZCamera.h"

namespace yunuGIAdapter
{
    class CameraAdapter : public yunuGI::ICamera
    {
    public:
        MZCamera mzCamera;

        CameraAdapter() {
            mzCamera.SetFrustum(0.25f * MathHelper::Pi, MZDX11Renderer::Instance.Get().GetAspectRatio(), 1.0f, 1000.0f);
        };
        virtual ~CameraAdapter() {};

        virtual void SetWorldTM(const yunuGI::Matrix4x4& tm) {
			XMMATRIX xmTM;

			xmTM.r[0].m128_f32[0] = tm.m11;
			xmTM.r[0].m128_f32[1] = tm.m12;
			xmTM.r[0].m128_f32[2] = tm.m13;
			xmTM.r[0].m128_f32[3] = tm.m14;

			xmTM.r[1].m128_f32[0] = tm.m21;
			xmTM.r[1].m128_f32[1] = tm.m22;
			xmTM.r[1].m128_f32[2] = tm.m23;
			xmTM.r[1].m128_f32[3] = tm.m24;

			xmTM.r[2].m128_f32[0] = tm.m31;
			xmTM.r[2].m128_f32[1] = tm.m32;
			xmTM.r[2].m128_f32[2] = tm.m33;
			xmTM.r[2].m128_f32[3] = tm.m34;

			xmTM.r[3].m128_f32[0] = tm.m41;
			xmTM.r[3].m128_f32[1] = tm.m42;
			xmTM.r[3].m128_f32[2] = tm.m43;
			xmTM.r[3].m128_f32[3] = tm.m44;

			mzCamera.SetWorldTM(xmTM);
            auto inverse = XMMatrixInverse(nullptr, reinterpret_cast<const XMMATRIX&>(tm));
            mzCamera.SetViewMatrix(XMMatrixInverse(nullptr, reinterpret_cast<const XMMATRIX&>(tm)));
        };
        virtual void SetFOV(float fov) {};
        virtual void SetNear(float near) {};
        virtual void SetFar(float far) {};
        virtual void SetAsMain() { mzCamera.SetMain(); };
    };
}
