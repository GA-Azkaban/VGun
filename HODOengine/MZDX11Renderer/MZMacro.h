#pragma once
#include <windows.h>

#if defined(DEBUG) | defined(_DEBUG)
inline void HR(HRESULT hr)
{
	if (FAILED(hr))							
	{								
		LPWSTR output;                                    	
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS | 
			FORMAT_MESSAGE_ALLOCATE_BUFFER, 
			NULL, 
			hr, 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR)&output, 
			0, 
			NULL);					        
		MessageBox(NULL, output, L"Error", MB_OK);		
	}
}
#else
inline void HR(HRESULT hr)
{
	
}
#endif