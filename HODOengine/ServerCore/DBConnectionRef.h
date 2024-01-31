#pragma once


namespace Horang
{
	class DBConnection;

	class DBConnectionRef
	{
	public:
		DBConnectionRef();
		~DBConnectionRef();

		DBConnection* operator->() const { return _dbConnection; }
		DBConnection operator*() const;

	private:
		DBConnection* _dbConnection;
	};
}