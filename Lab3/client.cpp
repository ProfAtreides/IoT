#include <iostream>
#include <string>
#include <ws2tcpip.h> // Winsock library
#include <random>
#include <chrono>
#include <thread>

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

void handleTraffic(SOCKET sock, int msgSize) {

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);



    int connResult = connect(sock, (sockaddr *) &hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        std::cerr << "Can't connect to server! Quitting" << std::endl;
        closesocket(sock);
        WSACleanup();
    }

    char result[4096];
    std::string data = randomString(msgSize);

    send(sock, std::to_string(msgSize).c_str(), BUFFER_SIZE, 0);

    if (msgSize > BUFFER_SIZE) {
        for (int i = 0; i < msgSize; i += BUFFER_SIZE) {
            std::string dataToSend = data.substr(i, BUFFER_SIZE);
            std::cout << dataToSend.length() << "\n";
            send(sock, dataToSend.c_str(), dataToSend.length(), 0);
        }
    } else {
        send(sock, data.c_str(), data.length(), 0);
    }


    std::cout << "Data sent...\n";
    std::cout << "Waiting for response...\n";

    int recvMsg = (recv(sock, result, 4096, 0));

    if (recvMsg == INVALID_SOCKET) {
        std::cerr << "Can't receive message!\n";
    } else {
        std::cout << "Received message: " << result << "\n";
    }

    closesocket(sock);
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
        int clientCount;
        std::cin >> clientCount;

        std::vector<std::thread> requests;

        for (int i = 0; i < clientCount; i++) {
            int msgSize;
            std::cin >> msgSize;

            SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock == INVALID_SOCKET) {
                std::cerr << "Can't create socket! Quitting" << std::endl;
                return 1;
            }

            // Fill in server information

            requests.emplace_back(handleTraffic, sock, std::ref(msgSize));
            requests.back().detach();
        }
    }

    WSACleanup();
}