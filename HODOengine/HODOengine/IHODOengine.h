#pragma once
#include "dllExporter.h"
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING

///
/// 231026
/// 엔진 프로세스를 클라와 연동하기 위한 인터페이스
/// 


class IHODOengine abstract
{
public:
	virtual void Initialize() abstract;
	virtual void Loop() abstract;
	virtual void Finalize() abstract;
	
};

extern "C" __declspec(dllexport) IHODOengine * CreateEngine();
extern "C" __declspec(dllexport) void ReleaseEngine(IHODOengine * instance);
