#pragma once

namespace Horang
{
	class GlobalQueue
	{
	public:
		GlobalQueue();
		~GlobalQueue();

	public:
		void Push(JobQueueRef jobQueue);
		JobQueueRef Pop();

	private:
		LockQueue<JobQueueRef> _jobQueues;
	};
}

