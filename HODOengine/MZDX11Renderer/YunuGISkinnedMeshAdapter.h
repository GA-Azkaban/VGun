#pragma once
#include <unordered_map>
#include <DirectXMath.h>
#include "YunuGraphicsInterface.h"
#include "IYunuGIRenderable.h"
#include "IRenderableObject.h"
#include "YunuGIRenderableAdapter.h"
#include "SkinnedMesh.h"
#include "MZUtility.h"

namespace yunuGIAdapter
{
	class SkinnedMeshAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::ISkinnedMesh
	{
	public:
		SkinnedMeshAdapter(SkinnedMesh* obj)
			:RenderableAdapter(obj), m_pSkinnedMesh(obj)
		{
		}
		~SkinnedMeshAdapter()
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

			m_pSkinnedMesh->SetWorldTM(xmTM);
		};
		virtual void SetActive(bool isActive) { m_pSkinnedMesh->SetActive(isActive); };
		virtual void LoadMesh(const char* fileName) { m_pSkinnedMesh->LoadMesh(fileName); };
		virtual void LoadDiffuseMap(const char* fileName) { m_pSkinnedMesh->SetDiffuseMap(fileName); }
		virtual void LoadNormalMap(const char* fileName) { m_pSkinnedMesh->SetNormalMap(fileName); }
		virtual void SetAnimation(const char* animID) { m_pSkinnedMesh->GetAnim().SetAnimation(animID); }
		virtual void SetAnimationRepeat(bool repeat) { m_pSkinnedMesh->GetAnim().SetLoopMode(repeat); }
		virtual void SetPickingMode(bool isPickingModeOn) { m_pSkinnedMesh->SetPickingMode(isPickingModeOn); }
		virtual void PlayAnimation() { m_pSkinnedMesh->GetAnim().Play(); }
	private:
		SkinnedMesh* m_pSkinnedMesh;
	};
}
