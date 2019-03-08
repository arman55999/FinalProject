#ifndef ___TXRUNNER__H_
#define ___TXRUNNER__H_

#include <thread>
#include <map>

#include "Database.h"
#include "Transaction.h"

class TxRunner_ {

public:
	TxRunner_(Database& db)
		: m_db(db)
	{}

	TxRunner_(TxRunner_&) = default;
	TxRunner_(TxRunner_&&) = default;
	TxRunner_& operator=(TxRunner_&) = default;
	~TxRunner_() = default;

	Database& get_db() {
		return m_db;
	}

	template <typename FUNCTION, typename... ARGS>
	void runTransactional(FUNCTION fun, ARGS... args) {

		static std::map<std::thread::id, Transaction> recursionMap;
		

		Transaction currentTransaction(m_db);	//creating transaction

		if (recursionMap.find(std::this_thread::get_id()) == recursionMap.end()) {
			recursionMap.insert(std::pair<std::thread::id,Transaction>(std::this_thread::get_id(), currentTransaction));
			currentTransaction.start();
		}
		
		
		try {
			fun(this, args...);
		}
		catch (...) {
			currentTransaction.abort();
			recursionMap.erase(std::this_thread::get_id());
			return;
		}

		currentTransaction.commit();
	}


private:
	Database& m_db;
};

#endif // ___TXRUNNER__H_


