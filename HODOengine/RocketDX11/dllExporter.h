#pragma once

#ifdef _GRAPHICS_EXPORT
#define ROCKET_API __declspec(dllexport)
#else
#define ROCKET_API __declspec(dllimport)
#endif //_EXPORT_API