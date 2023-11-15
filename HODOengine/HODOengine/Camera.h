#include "dllExporter.h"
#include "Component.h"
#include "IHodoGICamera.h"
#include <memory>

/// <summary>
/// 카메라는 화면에 그려질 그래픽 요소들을 출력하는 데 쓰이는 객체입니다.
/// 2023.11.09 김민정
/// </summary>

namespace hodoEngine
{
	class HODO_API Camera : public Component
	{
	public:
		Camera();
		static Camera* GetMainCamera();
		void SetMainCamera();
	protected:
		virtual void Update() override;
	private:
		std::unique_ptr<hodoGI::ICamera> _camera;
		static Camera* _mainCamera;
	};
}
