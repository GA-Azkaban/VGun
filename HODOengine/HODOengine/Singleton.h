#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING



template <typename T>
class Singleton
{
public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}

protected:
	// �� Ŭ������ ��ӹ��� ģ���鸸 ���� �� �ִ�
	Singleton() = default;

private:
	Singleton(const Singleton& rhs) = delete;
	Singleton(Singleton&& rhs) = delete;
	Singleton& operator=(const Singleton& rhs) = delete;
	Singleton& operator=(Singleton&& rhs) = delete;
};
