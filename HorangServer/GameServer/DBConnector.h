#pragma once
#include "Types.h"
#include <windows.h>
#include "DBBind.h"
#include <boost/locale.hpp>

namespace DB
{
	/// 문자열 관련

	inline auto ConvertUtf8toUtf16(const std::string& str)
	{
		return std::move(boost::locale::conv::utf_to_utf<WCHAR>(str));
	}

	template<int32 N>
	void WstrCpy(WCHAR(&v)[N], const std::string& str)
	{
		::wcsncpy_s(v, str.length() + 1, ConvertUtf8toUtf16(str).c_str(), _TRUNCATE);
	}
	template<int32 N>
	void WstrCpy(WCHAR(&v)[N], const std::wstring& str)
	{
		::wcsncpy_s(v, str.length() + 1, str.c_str(), _TRUNCATE);
	}


	/// DB Quary

	class SignIn : public Horang::DBBind<2, 2>
	{
	public:
		SignIn(Horang::DBConnection& conn)
			: DBBind(conn, L"SELECT uid, nickname FROM vgundb.user WHERE id = (?) AND password = (?);") {}

	public:
		template<int32 N>
		void In_Id(WCHAR(&v)[N]) { BindParam(0, v); };
		template<int32 N>
		void In_Id(const WCHAR(&v)[N]) { BindParam(0, v); };
		void In_Id(WCHAR* v, int32 count) { BindParam(0, v, count); };
		void In_Id(const WCHAR* v, int32 count) { BindParam(0, v, count); };
		template<int32 N>
		void In_Id(WCHAR(&v)[N], const std::string& str)
		{
			DB::WstrCpy(v, str);
			this->In_Id(v);
		}

	public:
		template<int32 N>
		void In_Password(WCHAR(&v)[N]) { BindParam(1, v); };
		template<int32 N>
		void In_Password(const WCHAR(&v)[N]) { BindParam(1, v); };
		void In_Password(WCHAR* v, int32 count) { BindParam(1, v, count); };
		void In_Password(const WCHAR* v, int32 count) { BindParam(1, v, count); };
		template<int32 N>
		void In_Password(WCHAR(&v)[N], const std::string& str)
		{
			DB::WstrCpy(v, str);
			this->In_Password(v);
		}

	public:
		void Out_Uid(int32& v) { BindCol(0, v); }
		template<int32 N>
		void Out_NickName(WCHAR(&v)[N]) { BindCol(1, v); }

	private:

	};

	class SignUp : public Horang::DBBind<3, 0>
	{
	public:
		SignUp(Horang::DBConnection& conn)
			: DBBind(conn, L"INSERT INTO user (id, password, nickname) VALUES(?, ?, ?)") {}

	public:
		template<int32 N>
		void In_Id(WCHAR(&v)[N]) { BindParam(0, v); };
		template<int32 N>
		void In_Id(const WCHAR(&v)[N]) { BindParam(0, v); };
		void In_Id(WCHAR* v, int32 count) { BindParam(0, v, count); };
		void In_Id(const WCHAR* v, int32 count) { BindParam(0, v, count); };

		template<int32 N>
		void In_Id(WCHAR(&v)[N], const std::string& str)
		{
			DB::WstrCpy(v, str);
			this->In_Id(v);
		}

	public:
		template<int32 N>
		void In_Password(WCHAR(&v)[N]) { BindParam(1, v); };
		template<int32 N>
		void In_Password(const WCHAR(&v)[N]) { BindParam(1, v); };
		void In_Password(WCHAR* v, int32 count) { BindParam(1, v, count); };
		void In_Password(const WCHAR* v, int32 count) { BindParam(1, v, count); };
		template<int32 N>
		void In_Password(WCHAR(&v)[N], const std::string& str)
		{
			DB::WstrCpy(v, str);
			this->In_Password(v);
		}

	public:
		template<int32 N>
		void In_NickName(WCHAR(&v)[N]) { BindParam(2, v); };
		template<int32 N>
		void In_NickName(const WCHAR(&v)[N]) { BindParam(2, v); };
		void In_NickName(WCHAR* v, int32 count) { BindParam(2, v, count); };
		void In_NickName(const WCHAR* v, int32 count) { BindParam(2, v, count); };
		template<int32 N>
		void In_NickName(WCHAR(&v)[N], const std::string& str)
		{
			DB::WstrCpy(v, str);
			this->In_NickName(v);
		}

	private:

	};
}