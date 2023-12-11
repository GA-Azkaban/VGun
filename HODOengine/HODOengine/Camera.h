#include "dllExporter.h"
#include "Component.h"
#include "../HODO3DGraphicsInterface/IHodoGICamera.h"
#include <memory>

/// <summary>
/// ī�޶�� ȭ�鿡 �׷��� �׷��� ��ҵ��� ����ϴ� �� ���̴� ��ü�Դϴ�.
/// 2023.11.09 �����
/// </summary>

namespace hodoData
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
		hodoGI::ICamera* _camera;
		static Camera* _mainCamera;
	};
}