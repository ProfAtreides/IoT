#include <iostream>
#include <string>
#include <ws2tcpip.h> // Winsock library
#include <random>
#include <chrono>

#pragma comment (lib, "ws2_32.lib") // Winsock library

#define BUFFER_SIZE 512

char *randomString(int n) {
    srand(time(NULL));
    char *result = new char[n];

    for (int i = 0; i < n; i++) {
        result[i] = rand() % 26 + 'a';
    }

    return result;
}

void handleClient(SOCKET clientSocket) {
    char *data;
    auto startTime = std::chrono::high_resolution_clock::now();

    recv(clientSocket, data, 4, 0);

    std::cout << "MESSAGE: " << data << "\n";
    int size = std::stoi(data);
    std::cout << "SIZE: " << size << "\n";

    recv(clientSocket, data, size, 0);

    std::string message(data, size);
    std::cout << "MESSAGE: " << message << "\n";

    auto recTime = std::chrono::high_resolution_clock::now();

    int bufferSent = send(clientSocket, message.c_str(), size, 0);

    auto sendTime = std::chrono::high_resolution_clock::now() - recTime;

    std::cout << "Bytes sent: " << bufferSent << "\n";
    std::cout << "Time taken to receive data: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(recTime - startTime).count() << "ns\n";
    std::cout << "Time taken to send data: " << std::chrono::duration_cast<std::chrono::nanoseconds>(sendTime).count()
              << "ns\n";

    closesocket(clientSocket);
}

int main() {
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
        return 1;
    }

    // Create socket
    char *result = new char[4096];

    while (true) {
        std::string data;
        int size;

        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Can't create socket! Quitting" << std::endl;
            return 1;
        }

        // Fill in server information
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(54000);
        inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);
        std::cin >> size;
        data = randomString(size);
        int connResult = connect(sock, (sockaddr *) &hint, sizeof(hint));
        if (connResult == SOCKET_ERROR) {
            std::cerr << "Can't connect to server! Quitting" << std::endl;
            closesocket(sock);
            WSACleanup();
            return 1;
        }

        std::cout << data << "\n";

        send(sock, std::to_string(size).c_str(), BUFFER_SIZE, 0);

        for (int i = 0; i < size; i += BUFFER_SIZE) {
            int bytesToSend = (i + BUFFER_SIZE) > size ? size - BUFFER_SIZE : BUFFER_SIZE;
            std::string dataToSend = data.substr(i, bytesToSend);
            std::cout << dataToSend.length() << "\n";
            send(sock, dataToSend.c_str(), dataToSend.length(), 0);
        }

        std::cout << "Data sent...\n";

        std::cout << "Waiting for response...\n";

        int recvMsg = (recv(sock, result, 4096, 0));

        if(recvMsg == INVALID_SOCKET){
            std::cerr << "Can't receive message!\n";
        }
        else {
            std::cout << "Received message: " << result << "\n";
        }


        closesocket(sock);
    }

    WSACleanup();
}