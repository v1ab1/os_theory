#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void WriterThread(int clientSocket) {
    while(true) {
        std::string message;
        std::getline(std::cin, message);
        send(clientSocket, message.c_str(), message.size(), 0);
    }
}

void ReaderThread(int clientSocket) {
    char buffer[1024];
    while(true) {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            break;
        }
        std::cout << "Клиент: " << buffer << std::endl;
    }
}

int main() {
    int yes = 1;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Невозможно создать сокет" << std::endl;
        return 1;
    }

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    std::cerr << "setsockopt failed" << std::endl;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Невозможно забиндить сокет" << std::endl;
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Невозможно слушать сокет" << std::endl;
        return 1;
    }

    std::cout << "Сервер работает на порту 808" << std::endl;

    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (clientSocket < 0) {
        std::cerr << "Нельзя принять входящее подключение" << std::endl;
        return 1;
    }

    std::cout << "Клиент подключен" << std::endl;

    std::thread reader(ReaderThread, clientSocket);
    std::thread writer(WriterThread, clientSocket);

    reader.join();
    writer.join();

    close(clientSocket);
    close(serverSocket);

    return 0;
}
