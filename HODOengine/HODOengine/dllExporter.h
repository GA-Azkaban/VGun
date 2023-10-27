#pragma once

#ifdef _EXPORT_API
#define HODO_API __declspec(dllexport)
#else
#define HODO_API __declspec(dllimport)
#endif //_EXPORT_API