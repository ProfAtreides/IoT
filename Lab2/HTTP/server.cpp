#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <thread>
#include <vector>

#define Buffer 1500
#pragma comment(lib, "ws2_32.lib")

void handleRequest(SOCKET clientSocket) {
    const char message[Buffer] = "Connected to localhost:80";
    char reply[Buffer] = {0};

    std::cout << "A Client Connected\n";
    send(clientSocket, message, strlen(message), NULL); // Send Client a Message

    recv(clientSocket, reply, sizeof(reply), NULL); // Just in case if the Client sends something, We Receive it.

    std::cout << "We've got a reply: " << reply << "\n";

    closesocket(clientSocket); // close the Client Socket now that our Work is Complete.
}

int main() {
    while (true) {
        WSADATA wsa;
        struct sockaddr_in server;
        SOCKET s;
        SOCKET clientS;
        int iResult;
        const char message[Buffer] = "Connected to localhost:80";
        char reply[Buffer] = {0};

        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            std::cerr << "WinsocK Failed" << std::endl;
            return 0;
        }
        s = socket(AF_INET, SOCK_STREAM, NULL);

        if (s == SOCKET_ERROR) {
            std::cerr << "Socket Error : " << WSAGetLastError() << "\n";
            return 0;
        } else if (s == INVALID_SOCKET) {
            std::cerr << "Socket Error : " << WSAGetLastError() << "\n";
            return 0;
        }

        std::cout << "Socket Created" << std::endl;

        server.sin_family = AF_INET;
        server.sin_port = htons(80);
        InetPton(AF_INET, _T("127.0.0.1"), &server.sin_addr);

        iResult = bind(s, (struct sockaddr *) &server, sizeof(server));
        if (iResult == SOCKET_ERROR) {
            std::cerr << "Bind Error " << WSAGetLastError() << std::endl;
            return 0;
        }

        std::cout << "Listening on : 127.0.0.1:80\n";
        iResult = listen(s, AF_INET);

        clientS = accept(s, NULL, NULL);
        if (clientS == SOCKET_ERROR) {
            std::cerr << "Accept Failed!" << WSAGetLastError() << std::endl;
            return 0;
        } else {
            std::thread t(handleRequest, clientS);
            t.detach();
        }

        closesocket(clientS);
        WSACleanup();
    }
}