#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

// Функция, которую будут выполнять потоки
void threadFunction(std::mutex& mtx, int& value) {
    // Захватываем мьютекс перед работой с общей переменной
    std::lock_guard<std::mutex> lock(mtx);
    
    // Изменяем аргумент (увеличиваем значение на единицу)
    value += 1;
    
    // Автоматический разблокировка мьютекса при выходе из области видимости lock_guard
}

int main() {
    const int NUM_THREADS = 5; // Кол-во потоков
    int sharedValue = 0; // Общий аргумент
    std::mutex mtx; // Мьютекс для синхронизации доступа к sharedValue
    std::vector<std::thread> threads; // Вектор для хранения потоков

    // Создаем и запускаем потоки
    for (int i = 0; i < NUM_THREADS; ++i) {
        // В каждый поток передаем ссылку на мьютекс и ссылку на sharedValue
        threads.emplace_back(threadFunction, std::ref(mtx), std::ref(sharedValue));
    }

    // Ждем завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    // Печатаем результат
    std::cout << "Resulting value: " << sharedValue << std::endl;
    
    return 0;
}
