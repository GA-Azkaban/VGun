#pragma once
#include "Job.h"
#include "LockQueue.h"

namespace Horang
{
	class JobQueue : public std::enable_shared_from_this<JobQueue>
	{
	public:
		void DoAsyncJob(Horang::JobRef&& job);

		void ClearJobs() { _jobs.Clear(); }

	public:
		void Push(Horang::JobRef job, bool pushOnly = false);
		void Execute();

	protected:
		LockQueue<Horang::JobRef> _jobs;
		Atomic<int32> _jobCount = 0;
	};
}

