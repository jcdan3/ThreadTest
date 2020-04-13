#pragma once
#include <mutex>
#include <vector>

class ThreadTester
{
public:
	std::mutex testMutex;
	std::string goAhead2();
	void goAhead();
	static void printLockedByThread(bool locked);

private:

	void doSomeCalculation();
	std::vector<double> vectorTester;
};

