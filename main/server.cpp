#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int READ_PORT = 8080;
const int WRITE_PORT = 8081;

void readMessages(int read_socket) {
    char buffer[1024];
    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        int bytes = recv(read_socket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;
        std::cout << "Client says: " << buffer << std::endl;
    }
}

void writeMessages(int write_socket) {
    std::string message;
    while (getline(std::cin, message)) {
        send(write_socket, message.c_str(), message.size(), 0);
    }
}

int main() {
    struct sockaddr_in address = {0};
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;

    int read_socket = socket(AF_INET, SOCK_STREAM, 0);
    int write_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (read_socket < 0 || write_socket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    address.sin_port = htons(READ_PORT);
    if (bind(read_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error on binding read_socket" << std::endl;
        return 1;
    }

    address.sin_port = htons(WRITE_PORT);
    if (bind(write_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error on binding write_socket" << std::endl;
        return 1;
    }

    if (listen(read_socket, 1) < 0 || listen(write_socket, 1) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return 1;
    }

    int server_read_socket = accept(read_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    int server_write_socket = accept(write_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    std::thread read_thread(readMessages, server_read_socket);
    std::thread write_thread(writeMessages, server_write_socket);

    read_thread.join();
    write_thread.join();

    close(read_socket);
    close(write_socket);
    close(server_read_socket);
    close(server_write_socket);

    return 0;
}
