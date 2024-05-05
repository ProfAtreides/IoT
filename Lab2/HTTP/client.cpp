#include <iostream>
#include <string>
#include <ws2tcpip.h> // Winsock library
#include <random>

#define BUFFER_SIZE 4096

#pragma comment (lib, "ws2_32.lib") // Winsock library


std::string randomString(int n)
{
    srand(time(NULL));
    std::string reuslt;

    for(int i = 0; i < n; i++) {
        reuslt.push_back(rand() % 26 + 'a');
    }

    return reuslt;
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
    char* result =  new char[BUFFER_SIZE];

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
        hint.sin_port = htons(80);
        inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);
        std::cin >> data >> size;
        int connResult = connect(sock, (sockaddr *) &hint, sizeof(hint));
        if (connResult == SOCKET_ERROR) {
            std::cerr << "Can't connect to server! Quitting" << std::endl;
            closesocket(sock);
            WSACleanup();
            return 1;
        }

        //send(sock, std::to_string(size).c_str(), 4, 0);
        send(sock, data.c_str(), size, 0);
        int bytesReceived = recv(sock, result, BUFFER_SIZE, 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Error in receiving response from server! Quitting\n";
        }

        std::string res(result);

        std::cout << "Result: " << res << "\n";

        closesocket(sock);
    }

    WSACleanup();
}