#pragma once
#include <unordered_map>
#include <DirectXMath.h>
#include "YunuGraphicsInterface.h"
#include "YunuGIRenderableAdapter.h"
#include "UIImage.h"
#include "MZUtility.h"

namespace yunuGIAdapter
{
	class UIImageAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::IUIImage
	{
	public:
		UIImageAdapter(UIImage* obj)
			:RenderableAdapter(obj), m_pUIImage(obj)
		{
		}
		virtual void SetWorldTM(const yunuGI::Matrix4x4& worldTM) {
			XMMATRIX xmTM;

			xmTM.r[0].m128_f32[0] = worldTM.m11;
			xmTM.r[0].m128_f32[1] = worldTM.m12;
			xmTM.r[0].m128_f32[2] = worldTM.m13;
			xmTM.r[0].m128_f32[3] = worldTM.m14;

			xmTM.r[1].m128_f32[0] = worldTM.m21;
			xmTM.r[1].m128_f32[1] = worldTM.m22;
			xmTM.r[1].m128_f32[2] = worldTM.m23;
			xmTM.r[1].m128_f32[3] = worldTM.m24;

			xmTM.r[2].m128_f32[0] = worldTM.m31;
			xmTM.r[2].m128_f32[1] = worldTM.m32;
			xmTM.r[2].m128_f32[2] = worldTM.m33;
			xmTM.r[2].m128_f32[3] = worldTM.m34;

			xmTM.r[3].m128_f32[0] = worldTM.m41;
			xmTM.r[3].m128_f32[1] = worldTM.m42;
			xmTM.r[3].m128_f32[2] = worldTM.m43;
			xmTM.r[3].m128_f32[3] = worldTM.m44;

			m_pUIImage->SetWorldTM(xmTM);
		};
		virtual void SetActive(bool isActive) { m_pUIImage->SetActive(isActive); };
		virtual void SetImage(LPCSTR fileName) { m_pUIImage->SetImage(fileName); };
		virtual void SetScreenSpace() { m_pUIImage->SetScreenSpace(); }
		virtual void SetWorldSpace() { m_pUIImage->SetWorldSpace(); }
		virtual void SetPickingMode(bool isPickingModeOn) { m_pUIImage->SetPickingMode(isPickingModeOn); }

	private:
		UIImage* m_pUIImage;
	};
}
