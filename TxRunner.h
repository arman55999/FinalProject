#ifndef ___TXRUNNER__H_
#define ___TXRUNNER__H_

#include <map>
#include <thread>
#include <mutex>
#include <vector>

#include "Database.h"
#include "Transaction.h"

std::mutex m_mutex;

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
		
		std::thread::id threadId = std::this_thread::get_id();

		if (recursionMap.find(threadId) == recursionMap.end()) {

			Transaction currentTransaction(m_db);

			m_mutex.lock();
			recursionMap.insert(std::pair<std::thread::id, Transaction>(threadId, currentTransaction));
			m_mutex.unlock();

			currentTransaction.start();

			try {
				fun(this, args...);
			}

			catch (...) {
				currentTransaction.abort();
				recursionMap.erase(threadId);
				return;
			}

			currentTransaction.commit();
			recursionMap.erase(threadId);			
		}
		else {
			fun(this, args...);
		}

	}


private:
	Database& m_db;
	std::map<std::thread::id, Transaction> recursionMap;
};

#endif // ___TXRUNNER__H_


