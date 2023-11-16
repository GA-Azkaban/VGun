#pragma once
#include <unordered_map>
#include <DirectXMath.h>
#include "YunuGraphicsInterface.h"
#include "IYunuGIRenderable.h"
#include "IRenderableObject.h"
#include "YunuGIRenderableAdapter.h"
#include "IYunuGIDebugCube.h"
#include "DebugCube.h"
#include "MZUtility.h"

namespace yunuGIAdapter
{
	class DebugCubeAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::IDebugCube
	{
	public:
		DebugCubeAdapter(DebugCube* obj)
			:RenderableAdapter(obj), m_pDebugCube(obj)
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

			m_pDebugCube->SetWorldTM(xmTM);
		};
		virtual void SetActive(bool isActive) { m_pDebugCube->SetActive(isActive); };
		virtual void SetFillModeSolid() { m_pDebugCube->SetFillModeSolid(); };
		virtual void SetFillModeWireframe() { m_pDebugCube->SetFillModeWireframe(); };
		virtual void SetColor(const yunuGI::Color& color) { m_pDebugCube->SetColor(XMFLOAT4(color.r, color.g, color.b, color.a)); };
	
	private:
		DebugCube* m_pDebugCube;
	};
}
