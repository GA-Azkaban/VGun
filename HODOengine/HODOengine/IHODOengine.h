#pragma once
#include "dllExporter.h"
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING

///
/// 231026
/// ���� ���μ����� Ŭ��� �����ϱ� ���� �������̽�
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
