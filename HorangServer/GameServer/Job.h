#pragma once

class IJob
{
public:
	virtual void Execute() abstract;

private:

};

using JobRef = std::shared_ptr<IJob>;

class JobQueue
{
public:
	void Push(JobRef job)
	{
		WRITE_LOCK;
		_jobs.push(job);

		// Todo
		std::cout << _jobs.size() << std::endl;
	}

	JobRef Pop()
	{
		WRITE_LOCK;
		if (_jobs.empty())
			return nullptr;

		JobRef ret = _jobs.front();
		_jobs.pop();
		return ret;
	}

private:
	USE_LOCK;
	Horang::Queue<JobRef> _jobs;
};

class Job : public IJob
{
public:
	virtual void Execute() override
	{

	}

private:

};

