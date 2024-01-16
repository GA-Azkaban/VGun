#pragma once
#include <stack>
#include <map>
#include <vector>
#include <set>
#include <unordered_map>

namespace Horang
{
	/*
		DeadLockProfiler
	*/

	class DeadLockProfiler
	{
	public:
		void PushLock(const char* name);
		void PopLock(const char* name);
		void CheckCycle();

	private:
		void Dfs(int32 index);

	private:
		std::unordered_map<const char*, int32> _nameToId;
		std::unordered_map<int32, const char*> _idToName;

		std::map<int32, std::set<int32>> _lockHistory;

		Mutex _lock;

	private:
		std::vector<int32> _discoveredOrder; // 노드가 발견된 순서 기록
		int32 _discoveredCount = 0; // 노드가 발견된 순서
		std::vector<bool> _finished; // Dfs(i) 번째가 종료됐는지 판별
		std::vector<int32> _parent;
	};

}