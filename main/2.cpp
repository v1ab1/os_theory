#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>

std::string g_str;
std::mutex g_str_mutex;

void appendChar(char key) {
    for (int i = 0; i < 10; ++i) {
        {
            std::lock_guard<std::mutex> lock(g_str_mutex);
            g_str += key;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    int numThreads;
    std::cout << "Введите количество потоков: ";
    std::cin >> numThreads;

    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(appendChar, 'A' + i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Итоговая строка: " << g_str << std::endl;

    return 0;
}
