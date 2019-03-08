#include <iostream>

#include "Database.h"
#include "TxRunner.h"

int main() {
	Database db;
	TxRunner_ tx_runner(db);
	std::cout << "First test.\n" << "In lambda function we calculate a + b function on every recursive call until 'a' is not 0.\n" << std::endl;
	std::function<void(TxRunner_* tx, int a, double b)> func = [&func](TxRunner_* tx, int a, double b) {
		if (a == 0)
			return;
		std::cout << "In lambda func: a+b=" << a + b << std::endl;

		// Call some write method on transaction database
		tx->get_db().write(a + b);
		tx->runTransactional(func, a - 1, b);
	};
	tx_runner.runTransactional(func, 5, 8.5);

	std::cout << "\nSecond test.\n" << "Here we submit a function that throws an exception\n";
	std::function<void(TxRunner_* tx, int a, int b)> func_error = [&func](TxRunner_* tx, int a, int b) {
		if (b == 0)
			throw std::runtime_error("Division to zero");

		// Call some write method on transaction database
		tx->get_db().write(a / b);
	};
	tx_runner.runTransactional(func_error, 10, 0);

	system("pause");
	return 0;
}