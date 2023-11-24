#include "HodoDebugCube.h"
#include "HodoGIObject.h"

hodoEngine::DebugCube::DebugCube()
	: Renderable<hodoGI::IDebugCube>(HodoGIObject::Instance().factory->CreateDebugCube())
{

}

hodoGI::IDebugCube* hodoEngine::DebugCube::Get()
{
	return Renderable<hodoGI::IDebugCube>::Get();
}
