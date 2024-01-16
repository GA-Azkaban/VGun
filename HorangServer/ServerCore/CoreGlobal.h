#pragma once

namespace Horang {
	class ThreadManager;
	class Memory;
	class SendBufferManager;
	class DeadLockProfiler;
	class DBConnectionPool;
}

extern Horang::ThreadManager* GThreadManager;
extern Horang::Memory* GMemory;
extern Horang::SendBufferManager* GSendBufferManager;
extern Horang::DeadLockProfiler* GDeadLockProfiler;
extern Horang::DBConnectionPool* GDBConnectionPool;