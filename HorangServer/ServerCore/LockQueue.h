#pragma once

namespace Horang
{
	template <typename T>
	class LockQueue
	{
	public:
		void Push(T object)
		{
			WRITE_LOCK;

			_objects.push(object);
		}

		T Pop()
		{
			WRITE_LOCK;

			if (_objects.empty())
				return T();

			T ret = _objects.front();
			_objects.pop();

			return ret;
		}

		void PopAll(Vector<T>& objects)
		{
			WRITE_LOCK;

			while (T object = this->Pop())
				objects.push_back(object);
		}

		void Clear()
		{
			WRITE_LOCK;

			_objects = Queue<T>();
		}

	private:
		USE_LOCK;
		Queue<T> _objects;
	};
}
