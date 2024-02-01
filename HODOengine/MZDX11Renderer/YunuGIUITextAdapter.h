#pragma once
#include <unordered_map>
#include <DirectXMath.h>
#include "YunuGraphicsInterface.h"
#include "YunuGIRenderableAdapter.h"
#include "UIText.h"
#include "MZUtility.h"

namespace yunuGIAdapter
{
	class UITextAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::IUIText
	{
	public:
		UITextAdapter(UIText* obj)
			:RenderableAdapter(obj), m_pUIText(obj)
		{
		}
		/*virtual ~UITextAdapter()
		{
			delete m_pUIText;
		}*/
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

			m_pUIText->SetWorldTM(xmTM);
		};
		virtual void SetActive(bool isActive) { m_pUIText->SetActive(isActive); };
		virtual void SetText(TCHAR* text, ...) { 
			TCHAR buffer[1024] = L"";
			va_list vl;
			va_start(vl, text);
			_vstprintf(buffer, 1024, text, vl);
			va_end(vl);

			m_pUIText->SetText(buffer); 
		};
		virtual const std::string GetText() { return m_pUIText->GetText(); }
		virtual void SetScreenSpace() { m_pUIText->SetScreenSpace(); }
		virtual void SetWorldSpace() { m_pUIText->SetWorldSpace(); }
		virtual void SetPickingMode(bool isPickingModeOn) { m_pUIText->SetPickingMode(isPickingModeOn); }

	private:
		UIText* m_pUIText;
	};
}
