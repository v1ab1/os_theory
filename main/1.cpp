#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

void threadFunction(int &arg, std::mutex &mu) {
    std::lock_guard<std::mutex> guard(mu);
    arg += 5;
    std::cout << "ID потока: " << std::this_thread::get_id() << ", измененное значение: " << arg << std::endl;
}

int main() {
    const int numThreads = 5;
    int argument = 0;

    std::mutex mu;

    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(threadFunction, std::ref(argument), std::ref(mu));
    }

    for (std::thread &th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    std::cout << "Финальное значение после остановки всех потоков: " << argument << std::endl;

    return 0;
}
