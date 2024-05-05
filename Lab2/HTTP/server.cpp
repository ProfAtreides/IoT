// Nessecary Includes.
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>

// Buffer
#define Buffer 1500
// A Pragma comment. Loading Library
#pragma comment(lib, "ws2_32.lib")

int main() {
    while (true) {
        // Variables and Declarations
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

        server.sin_family = AF_INET; // Using AF_INET Address Family.
        server.sin_port = htons(80); // Defining PORT
        InetPton(AF_INET, _T("127.0.0.1"), &server.sin_addr); // Defining The Network Address to Run the Server on

        iResult = bind(s, (struct sockaddr *) &server, sizeof(server)); // binding the Host Address and Port Number
        if (iResult == SOCKET_ERROR) // If Bind gives Error
        {
            std::cerr << "Bind Error " << WSAGetLastError() << std::endl;
            return 0;
        }

        std::cout << "Listening on : 127.0.0.1:80\n";
        iResult = listen(s, AF_INET); // Then Actually Start Listening for incoming Connections.

        clientS = accept(s, NULL, NULL); // Accept a Connection on a new Socket made for the Client.
        if (clientS == SOCKET_ERROR) { // if Accepting Connection is a Error
            std::cerr << "Accept FAiled!" << WSAGetLastError() << std::endl;
            return 0;
        } else {
            std::cout << "A Client Connected\n";
            send(clientS, message, strlen(message), NULL); // Send Client a Message
        }

        recv(clientS, reply, sizeof(reply), NULL); // Just in case if the Client sends something, We Receive it.

        std::cout << "We've got a reply: " << reply << "\n";

        closesocket(clientS); // close the Client Socket now that our Work is Complete.
        WSACleanup(); // Clean Windows Socket API.
    }
}