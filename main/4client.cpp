#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void SendToServer(int serverSocket) {
    while(true) {
        std::string message;
        std::getline(std::cin, message);
        send(serverSocket, message.c_str(), message.size(), 0);
    }
}

void ReceiveFromServer(int serverSocket) {
    char buffer[1024];
    while(true) {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(serverSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            break;
        }
        std::cout << "Сервер: " << buffer << std::endl;
    }
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Невозможно создать сокет" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        std::cerr << "Неправильный адрес" << std::endl;
        return 1;
    }

    if (connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Ошибка подключения" << std::endl;
        return 1;
    }

    std::cout << "Подключено к серверу!" << std::endl;

    std::thread senderThread(SendToServer, serverSocket);
    std::thread receiverThread(ReceiveFromServer, serverSocket);
    senderThread.join();
    receiverThread.join();

    close(serverSocket);

    return 0;
}
