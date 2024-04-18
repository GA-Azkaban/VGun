#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <locale>

namespace MZUtility
{
	inline std::wstring GetWString(const std::string& arg) { return GetWString(arg.c_str()); }
	inline std::wstring GetWString(const char* arg)
	{
		// Convert std::string to std::wstring using MultiByteToWideChar
		int wstrSize = MultiByteToWideChar(CP_UTF8, 0, arg, -1, nullptr, 0);
		std::wstring wstr(wstrSize, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, arg, -1, &wstr[0], wstrSize);
		return wstr;
	}
	inline std::string GetString(const std::wstring& arg) { return GetString(arg.c_str()); }
	inline std::string GetString(const wchar_t* arg)
	{
		// Convert std::wstring to std::string using WideCharToMultiByte
		int strSize = WideCharToMultiByte(CP_UTF8, 0, arg, -1, nullptr, 0, nullptr, nullptr);
		std::string str(strSize, '\0');
		WideCharToMultiByte(CP_UTF8, 0, arg, -1, &str[0], strSize, nullptr, nullptr);
		return str;
	}
}
