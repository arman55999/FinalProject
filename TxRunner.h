#ifndef ___TXRUNNER__H_
#define ___TXRUNNER__H_

#include <map>
#include <thread>

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

		static std::multimap<std::thread::id, Transaction> recursionMap;
		

		Transaction currentTransaction(m_db);	//creating transaction

		if (recursionMap.count(std::this_thread::get_id()) == 0) {			
			currentTransaction.start();
		}
		
		recursionMap.insert(std::pair<std::thread::id, Transaction>(std::this_thread::get_id(), currentTransaction));
		
		try {
			fun(this, args...);

			recursionMap.erase(recursionMap.find(std::this_thread::get_id()));

			if(recursionMap.find(std::this_thread::get_id()) == recursionMap.end()){
				currentTransaction.commit();
				return;
			}
			
		}
		catch (...) {
			currentTransaction.abort();
			recursionMap.erase(std::this_thread::get_id());
			return;
		}

		

	}


private:
	Database& m_db;
};

#endif // ___TXRUNNER__H_


