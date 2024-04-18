#pragma once
///
/// 렌더러의 인터페이스
/// 2023.06.19 MJKIM

namespace MZRenderer
{
	class I3DRenderer
	{
	public:
		I3DRenderer() {};
		virtual ~I3DRenderer() {};

		// 엔진 초기화
		virtual bool Initialize() abstract;
		virtual void SetOutputWindow(unsigned int hWnd) abstract;
		// 업데이트
		virtual void Update(float deltaTime) abstract;

		// 렌더
		virtual void Render() abstract;

		// 엔진 종료
		virtual void Finalize() abstract;

		// 창 변환 관련
		virtual void ResizeResolution(unsigned int width, unsigned int height) abstract;
	};

}
