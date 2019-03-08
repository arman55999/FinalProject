#ifndef ___DATABASE__H_
#define ___DATABASE__H_

#include <iostream>

class Database
{
public:
	void write(int info) {
		std::cout << info << " written in db" << std::endl;
	}
};

#endif // ___DATABASE__H_
