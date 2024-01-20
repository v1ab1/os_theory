#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

// Функция потока, принимающая ссылку на аргумент и изменяющая его
void threadFunction(int &arg, std::mutex &mu) {
    std::lock_guard<std::mutex> guard(mu);
    // Изменяем аргумент
    arg += 5;
    // Распечатываем информацию о потоке и измененном аргументе
    std::cout << "ID потока: " << std::this_thread::get_id() << ", измененное значение: " << arg << std::endl;
}

int main() {
    // Количество потоков
    const int numThreads = 5;
    // Исходный аргумент
    int argument = 0;

    // Мьютекс для синхронизации доступа к аргументу
    std::mutex mu;

    // Вектор для хранения потоков
    std::vector<std::thread> threads;

    // Запускаем несколько потоков
    for (int i = 0; i < numThreads; ++i) {
        // Создаем и запускаем поток, передаем аргумент по ссылке
        threads.emplace_back(threadFunction, std::ref(argument), std::ref(mu));
    }

    // Дожидаемся завершения всех потоков
    for (std::thread &th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    // Печатаем конечное значение аргумента после работы всех потоков
    std::cout << "Финальное значение после остановки всех потоков: " << argument << std::endl;

    return 0;
}
