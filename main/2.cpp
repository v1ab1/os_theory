#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

// Глобальная строковая переменная
std::string g_sharedString;
// Мьютекс для синхронизации доступа к глобальной переменной
std::mutex g_mutex;

// Функция потока
void appendCharToGlobalString(char key) {
    for (int i = 0; i < 10; ++i) {
        {
            // Блокировка мьютекса перед внесением изменений в строку
            std::lock_guard<std::mutex> lock(g_mutex);
            // Добавление символа в глобальную строку
            g_sharedString.push_back(key);
        }
        // Ждем 1 секунду
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    int numThreads;
    std::cout << "Enter the number of threads: ";
    std::cin >> numThreads;

    std::vector<std::thread> threads;

    // Создаем и запускаем потоки
    for (int i = 0; i < numThreads; ++i) {
        // Передаем уникальный символ каждому потоку
        threads.emplace_back(appendCharToGlobalString, 'A' + i);
    }

    // Ждем завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    // Выводим результат
    std::cout << "Final string: " << g_sharedString << std::endl;

    return 0;
}
