#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <chrono>

const int DB_MAX_SIZE = 10; // Максимальный размер базы данных
const int ITEMS_TO_PRODUCE = 5; // Количество элементов для записи
const int ITEMS_TO_CONSUME = 5; // Количество элементов для чтения

// База данных
std::queue<char> database;
std::mutex db_mutex;
std::condition_variable db_not_full;
std::condition_variable db_not_empty;

// Поток писатель
void producer(char item) {
    int produced = 0;
    while (produced < ITEMS_TO_PRODUCE) {
        std::unique_lock<std::mutex> lock(db_mutex);
        db_not_full.wait(lock, [] { return database.size() < DB_MAX_SIZE; });

        // Записываем элемент в базу данных
        database.push(item);
        std::cout << "Produced: " << item << std::endl;
        produced++;

        lock.unlock();
        db_not_empty.notify_one();

        // Имитация небольшого случайного задержки
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 10));
    }
}

// Поток читатель
void consumer() {
    int consumed = 0;
    while (consumed < ITEMS_TO_CONSUME) {
        std::unique_lock<std::mutex> lock(db_mutex);
        db_not_empty.wait(lock, [] { return !database.empty(); });

        // Считываем элемент из базы данных
        char item = database.front();
        database.pop();
        std::cout << "Consumed: " << item << std::endl;
        consumed++;
        
        lock.unlock();
        db_not_full.notify_one();

        // Имитация небольшого случайного задержки
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 10));
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // Создаем и запускаем потоки писатели
    std::thread p1(producer, 'A');
    std::thread p2(producer, 'B');

    // Создаем и запускаем потоки читатели
    std::thread c1(consumer);
    std::thread c2(consumer);

    // Ждем завершения всех потоков
    p1.join();
    p2.join();
    c1.join();
    c2.join();

    // Проверяем, пуста ли база данных
    if (database.empty()) {
        std::cout << "Database is empty after all operations." << std::endl;
    } else {
        std::cout << "Database is not empty after all operations." << std::endl;
    }

    return 0;
}
