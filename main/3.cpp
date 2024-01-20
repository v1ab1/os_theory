#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <chrono>

const int DB_MAX_SIZE = 10;
const int ITEMS_TO_PRODUCE = 7;
const int ITEMS_TO_CONSUME = 5;

std::queue<char> database;
std::mutex db_mutex;
std::condition_variable db_not_full;
std::condition_variable db_not_empty;

void producer(char item) {
    int produced = 0;
    while (produced < ITEMS_TO_PRODUCE) {
        std::unique_lock<std::mutex> lock(db_mutex);
        db_not_full.wait(lock, [] { return database.size() < DB_MAX_SIZE; });

        database.push(item);
        std::cout << "Записано: " << item << std::endl;
        produced++;

        lock.unlock();
        db_not_empty.notify_one();

        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 10));
    }
}

void consumer() {
    int consumed = 0;
    while (consumed < ITEMS_TO_CONSUME) {
        std::unique_lock<std::mutex> lock(db_mutex);
        db_not_empty.wait(lock, [] { return !database.empty(); });

        char item = database.front();
        database.pop();
        std::cout << "Прочитано: " << item << std::endl;
        consumed++;
        
        lock.unlock();
        db_not_full.notify_one();

        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 10));
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    std::thread p1(producer, 'A');
    std::thread p2(producer, 'B');

    std::thread c1(consumer);
    std::thread c2(consumer);

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    if (database.empty()) {
        std::cout << "База данных пуста" << std::endl;
    } else {
        std::cout << "База данных НЕ пуста" << std::endl;
    }

    return 0;
}
