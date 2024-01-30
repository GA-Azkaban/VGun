#pragma once
#include "../HODO3DGraphicsInterface/ICamera.h"
#include "MZCamera.h"
#include "MZDX11Renderer.h"

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

		virtual void SetPositionAndRotation(const Vector3& pos, const Quaternion& rot)
		{			
			mzCamera.SetPosition(pos);
			mzCamera.SetRotation(rot);

			mzCamera.UpdateViewMatrix();
		}

		virtual void SetWorldTM(const Matrix& matrix) {
			mzCamera.SetWorldTM(matrix);
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
