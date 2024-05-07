#pragma once


namespace Horang
{
	class IJob
	{
	public:
		virtual void Execute() abstract;

	private:

	};


	class Job : public IJob
	{


	};

}

