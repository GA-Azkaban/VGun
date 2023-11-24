#include "Renderable.h"
#include "../HODO3DGraphicsInterface/IDebugCube.h"
#include "dllExporter.h"

namespace hodoEngine
{
	class HODO_API DebugCube : public Renderable<hodoGI::IDebugCube>
	{
	public:
		DebugCube();
		hodoGI::IDebugCube* Get();
	};
}
