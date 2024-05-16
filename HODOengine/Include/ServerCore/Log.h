#pragma once

using Horang::Log;

namespace Horang
{
	enum class Color
	{
		BLACK,
		WHITE,
		RED,
		GREEN,
		BLUE,
		YELLOW,
	};

	class LogStream
	{
	public:
		LogStream(Color color = Color::WHITE)
			: _color(color) {}

		template<typename Arg>
		LogStream& operator<<(const Arg& arg)
		{
			Log::DebugLog(arg, _color);
			return *this;
		}

	private:
		Color _color;
	};

	LogStream DebugLog(Color color = Color::WHITE);

	class Log
	{
	public:
		Log();
		~Log();

	public:
		template<typename Arg, typename... Args>
		static void DebugLog(const Arg& arg, Args&&... args)
		{
			if constexpr (sizeof...(args) == 0)
				DebugLog(L"", Color::WHITE, arg);
			else
				DebugLog(arg, Color::WHITE, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void DebugLog(Horang::Color color, Args&&... args)
		{
			DebugLog(L"", color, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void DebugLog(Args&&... args)
		{
			DebugLog(L"", Color::WHITE, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		static void DebugLog(const T& delimiter, Color color, Args&&... args)
		{
			SetColor(color);
			size_t count = sizeof...(args);
			DebugLog(delimiter, count, std::forward<Args>(args)...);
		}

	private:
		template<typename T, typename Arg, typename... Rest>
		static void DebugLog(const T& delimiter, size_t count, Arg&& arg, Rest&&... rest)
		{
#ifdef _DEBUG
			if constexpr (std::is_same_v<std::decay_t<Arg>, std::string>)
				std::cout << arg;
			else
				std::wcout << std::forward<Arg>(arg);

			if (count > 1)
				std::wcout << delimiter;
			DebugLog(delimiter, count - 1, std::forward<Rest>(rest)...);
#endif // _DEBUG
		}

		template<typename T>
		static void DebugLog(const T& delimiter, size_t count)
		{
			std::wcout << std::endl;
			Log::SetColor(Horang::Color::WHITE);
		}

	private:
		static void SetColor(Color color);

	private:
		static HANDLE _stdOut;
	};
};

using Horang::Color;