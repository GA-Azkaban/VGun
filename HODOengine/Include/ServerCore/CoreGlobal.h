#pragma once

namespace Horang
{
	class ThreadManager;
	class Memory;
	class SendBufferManager;
	class DeadLockProfiler;
	class DBConnectionPool;
	class GlobalQueue;
}

extern Horang::ThreadManager* GThreadManager;
extern Horang::Memory* GMemory;
extern Horang::SendBufferManager* GSendBufferManager;
extern Horang::DeadLockProfiler* GDeadLockProfiler;
extern Horang::DBConnectionPool* GDBConnectionPool;
extern Horang::GlobalQueue* GGlobalQueue;