#pragma once
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <wrl.h>

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#include <cassert>
#include <fstream>
#include <vector>
#include <map>
#include "MathHelper.h"
#include "LightHelper.h"
#include "InputLayouts.h"
#include "RasterizerState.h"

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)									\
	{									\
		HRESULT hr = (x);						\
		if(FAILED(hr))							\
		{								\
			LPWSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |		\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,			\
				NULL,						\
				hr,						\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,				\
				0,						\
				NULL);					        \
			MessageBox(NULL, output, L"Error", MB_OK);		\
		}								\
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

#define ReleaseCOM(x) { if(x){ x.Reset(); x = nullptr; } }
#define SafeDelete(x) { delete x; x = 0; }
#define SafeRelease(x) { x->Release(); x = 0; }