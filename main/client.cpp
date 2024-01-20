#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const char* SERVER_ADDRESS = "127.0.0.1";
const int SERVER_READ_PORT = 8080;
const int SERVER_WRITE_PORT = 8081;

void readMessages(int read_socket) {
    char buffer[1024];
    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        int bytes = recv(read_socket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;
        std::cout << "Server says: " << buffer << std::endl;
    }
}
void writeMessages(int write_socket) {
    std::string message;
    while (getline(std::cin, message)) {
        send(write_socket, message.c_str(), message.size(), 0);
    }
}

int main() {
    struct sockaddr_in server_address;

    int read_socket = socket(AF_INET, SOCK_STREAM, 0);
    int write_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (read_socket < 0 || write_socket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_READ_PORT);
    inet_pton(AF_INET, SERVER_ADDRESS, &server_address.sin_addr);

    if (connect(read_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed for read_socket" << std::endl;
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_WRITE_PORT);
    inet_pton(AF_INET, SERVER_ADDRESS, &server_address.sin_addr);
    
    if (connect(write_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed for write_socket" << std::endl;
        return 1;
    }

    std::thread read_thread(readMessages, read_socket);
    std::thread write_thread(writeMessages, write_socket);

    read_thread.join();
    write_thread.join();
    
    close(read_socket);
    close(write_socket);

    return 0;
}
