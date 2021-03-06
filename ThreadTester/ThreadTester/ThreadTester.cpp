#include "ThreadTester.h"
#include <thread>
#include <iostream>
#include <chrono>
#include <future>


std::string ThreadTester::goAhead2()
{
    goAhead();
    return std::string("all done");
}

void ThreadTester::goAhead()
{
    testMutex.lock();
    printLockedByThread(true);

    doSomeCalculation();

    testMutex.unlock();
    printLockedByThread(false);
}

void ThreadTester::printLockedByThread(bool locked)
{
    std::thread::id this_id = std::this_thread::get_id();

    std::string mess;
    if (locked)
        mess = " has locked the resource";
    else
        mess = " has unlocked the resource";

    std::cout << "Thread " << this_id << mess << std::endl;
}

void ThreadTester::doSomeCalculation()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();

    std::vector<double> calcVector;
    for (size_t i = 0; i <= 2000000; ++i)
    {
        double t = std::rand() % 1000;
        calcVector.push_back(t);
    }
    
    for (auto& e : calcVector)
    {
        if (e <= 5 && e >= 2)
            ++e;
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << "Calculation time: " << elapsed_seconds.count() << " seconds for thread "<< std::this_thread::get_id() << std::endl;
}



void calculateWithoutAsync()
{
    std::unique_ptr<ThreadTester> asynctester = std::make_unique<ThreadTester>();

    std::unique_ptr<ThreadTester> asynctester2 = std::make_unique<ThreadTester>();

    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();

    std::cout << "\n";
    std::cout << "Starting test without async" << std::endl;

    asynctester->goAhead2();
    asynctester2->goAhead2();

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << "Calculation time: " << elapsed_seconds.count() << " seconds without async" << std::endl;
}


void calculateWithAsync()
{
    std::shared_ptr<ThreadTester> asynctester = std::make_shared<ThreadTester>();

    std::shared_ptr<ThreadTester> asynctester2 = std::make_shared<ThreadTester>();

    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();

    std::cout << "\n";
    std::cout << "Starting test with async" << std::endl;

    std::future<std::string> first = std::async(std::launch::async, &ThreadTester::goAhead2, asynctester);

    std::future<std::string> second = std::async(std::launch::async, &ThreadTester::goAhead2, asynctester2);

    // Will block till data is available in future<std::string> object.

    std::cout << "Waiting for threads to finish" << std::endl;
    std::string firstResult = first.get();
    std::string secondResult = second.get();

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << "Calculation time: " << elapsed_seconds.count() << " seconds with asyc " << std::endl;
}

void doAsyncTest()
{
    calculateWithoutAsync();
    calculateWithAsync();
}

int main()
{

    std::cout << "Testing on multithreading" << std::endl;
    std::shared_ptr<ThreadTester> tester = std::make_shared<ThreadTester>();

    std::shared_ptr<ThreadTester> tester2 = tester;

    std::thread firsthread(&ThreadTester::goAhead,tester);
    std::thread secondthread(&ThreadTester::goAhead, tester2);

    firsthread.join();
    std::cout << "First Thread is over (thread as joined in main)" << std::endl;

    secondthread.join();
    std::cout << "Second Thread is over (thread as joined in main)" << std::endl;

    std::cout << "\n\n" << "Testing on async and future" << std::endl;

    doAsyncTest();

    int x;
    std::cin >> x;
}

 