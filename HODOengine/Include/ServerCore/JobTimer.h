#pragma once

namespace Horang
{
	struct JobData
	{
		JobData(std::weak_ptr<Horang::JobQueue> owner, Horang::JobRef job)
			: owner(owner), job(job)
		{
		}

		std::weak_ptr<Horang::JobQueue> owner;
		Horang::JobRef job;
	};

	struct TimerJob
	{
		bool operator<(const TimerJob& rhs) const
		{
			return executeTick > rhs.executeTick;
		}

		uint64 executeTick = 0;
		JobData* jobData = nullptr;
	};


	class JobTimer
	{
	public:
		void Reserve(uint64 tickAfter, std::weak_ptr<JobQueue> owner, Horang::JobRef job);
		void Distribute(uint64 now);
		void Clear();

	private:
		USE_LOCK;
		Horang::PriorityQueue<TimerJob> _jobs;
		Atomic<bool> _distributing = false;
	};

}