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

	void Clear()
	{
		WRITE_LOCK;
		_jobs = decltype(_jobs)();
	}

	int32 Size() 
	{
		READ_LOCK;
		return _jobs.size(); 
	}
	
	bool Empty() 
	{
		READ_LOCK;
		return _jobs.empty(); 
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

