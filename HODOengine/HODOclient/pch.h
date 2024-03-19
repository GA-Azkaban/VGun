#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG

#pragma comment(lib,"ServerCore.lib")
#pragma comment(lib,"libprotobufd.lib")

#else
#pragma comment(lib,"Release\\ServerCore.lib")
#pragma comment(lib,"Release\\libprotobuf.lib")

#endif

#include "CorePch.h"
