#include <iostream>
#include <string>
#include <ws2tcpip.h>
#include <cmath>
#include <chrono>

#pragma comment (lib, "ws2_32.lib")

int main() {
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
        return 1;
    }

    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Can't create socket! Quitting" << std::endl;
        WSACleanup();
        return 1;
    }

    std::cout << "Creating server socket..\n";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr *) &hint, sizeof(hint));
    listen(listening, SOMAXCONN);
    sockaddr_in client;
    int clientSize = sizeof(client);


    while (true) {
        char* data;
        SOCKET clientSocket = accept(listening, (sockaddr *) &client, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Can't accept client connection! Quitting" << std::endl;
            closesocket(listening);
            WSACleanup();
            return 1;
        }

        auto startTime = std::chrono::high_resolution_clock::now();

        recv(clientSocket, data, 4, 0);

        std::cout << "MESSAGE: " << data << "\n";
        int size = std::stoi(data);
        std::cout << "SIZE: " << size <<"\n";

        recv(clientSocket, data, size, 0);

        std::string message(data, size);
        std::cout << "MESSAGE: " << message << "\n";

        auto recTime = std::chrono::high_resolution_clock::now();

        int bufferSent = send(clientSocket,  message.c_str(), size, 0);

        auto sendTime = std::chrono::high_resolution_clock::now() - recTime;

        std::cout << "Time taken to receive data: " << std::chrono::duration_cast<std::chrono::nanoseconds >(recTime - startTime).count() << "ns\n";
        std::cout << "Time taken to send data: " << std::chrono::duration_cast<std::chrono::nanoseconds >(sendTime).count() << "ns\n";

        closesocket(clientSocket);
    }

}