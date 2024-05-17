#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>

using namespace std;

int count1=9999999;
int simpleCounter = 0;
int mutexCounter = 0;
std::atomic<int> atomicCounter(0);
std::mutex m;

void SimpleIncrementation() {
	for (int i = 0; i <= count1; i++) {
		simpleCounter++;
	}
}

void MutexIncrementation() {
	for (int i = 0; i <= count1; i++) {
		std::lock_guard<std::mutex> lock(m);
		mutexCounter++;
	}
}

void AtomicIncrementation() {
	for (int i = 0; i <= count1; i++) {
		atomicCounter++;
	}
}

int main()
{
	std::thread t1(SimpleIncrementation);
	std::thread t2(SimpleIncrementation);
	t1.join();
	t2.join();
	std::cout << "Результат обычного инкрементирования" << simpleCounter << std::endl;

	std::thread t3(MutexIncrementation);
	std::thread t4(MutexIncrementation);
	t3.join();
	t4.join();
	std::cout << "Результат с использованием мьютекса: " << mutexCounter << std::endl;

	std::thread t5(AtomicIncrementation);
	std::thread t6(AtomicIncrementation);
	t5.join();
	t6.join();
	std::cout << "Результат с использованием атомиков: " << atomicCounter << std::endl;

}