#ifndef ___TRANSACTION__H_
#define ___TRANSACTION__H_

#include <iostream>

#include "Database.h"

class Transaction {
public:
	Transaction(Database& db)
		: m_db(db)
	{}

	Transaction(Transaction&) = default;
	Transaction(Transaction&&) = default;
	Transaction& operator=(Transaction&) = default;
	~Transaction() = default;

	/// Starts the TX. Illegal to call if the TX already started.
	void start() {
		std::cout << "TRANSACTION STARTED" << std::endl;
	}

	/// Commits a started TX. Illegal to call if TX was not yet started or is already committed/aborted.
	void commit() {
		std::cout << "TRANSACTION COMMITED" << std::endl;
	}

	/// Aborts a started TX. Illegal to call if TX was not yet started or is already committed/aborted.
	void abort() {
		std::cout << "TRANSACTION ABORTED" << std::endl;
	}

private:
	Database& m_db;
};

#endif // ___TRANSACTION__H_