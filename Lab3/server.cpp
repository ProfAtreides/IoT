#include <iostream>
#include <string>
#include <ws2tcpip.h>
#include <cmath>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <array>
#include <fstream>

#pragma comment (lib, "ws2_32.lib")

#define BUFFER_SIZE 512

#define MIN_USERS 3
#define MAX_USERS 5

std::mutex bufferMutex, userCountMutex;
std::condition_variable userCountCV, bufferCV;

int users = 0;
bool bufferInUse = false;

class Client {
private:
   inline static unsigned int totalAmount = 0;
public:
    SOCKET clientSocket;
    std::string data;
    unsigned int number;
    int dataSize;

    Client(SOCKET _clientSocket){
        this->clientSocket = _clientSocket;
        number = ++totalAmount;
        data = "";
        dataSize = -1;
    }
};

void handleClients(SOCKET clientSocket, std::ofstream& log) {
    auto _client = new Client(clientSocket);

    users++;

    std::unique_lock userCountLock(userCountMutex);

    char msgSize[BUFFER_SIZE];
    char msg[BUFFER_SIZE];

    std::cout << "Client number " << _client->number << " connected...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds (10));

    userCountCV.wait(userCountLock, []{return users >= MIN_USERS;} );

    int bytesReceivedSize = (recv(_client->clientSocket, msgSize, BUFFER_SIZE, 0));

    int size = 0;

    if (bytesReceivedSize == INVALID_SOCKET) {
        std::cout << "Recv failed " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Received size " << bytesReceivedSize<<  " bytes\n";
        size = std::stoi(msgSize);
    }

    if(size > BUFFER_SIZE){
        for(int i = 0; i < size; i+=BUFFER_SIZE){
            int bytesToReceive = (i + BUFFER_SIZE) > size ? size - BUFFER_SIZE : BUFFER_SIZE;
            int bytesReceivedMsg = (recv(_client->clientSocket, msg, bytesToReceive, 0));
            if (bytesReceivedMsg == INVALID_SOCKET) {
                std::cout << "Recv failed " << WSAGetLastError() << std::endl;
            } else {
                std::cout << "Received msg " << bytesReceivedMsg<<  " bytes\n";
                _client->data += std::string(msg, bytesReceivedMsg);
            }
        }
    }
    else {
        int bytesReceivedMsg = (recv(_client->clientSocket, msg, size, 0));
        if (bytesReceivedMsg == INVALID_SOCKET) {
            std::cout << "Recv failed " << WSAGetLastError() << std::endl;
        } else {
            std::cout << "Received msg " << bytesReceivedMsg<<  " bytes\n";
            _client->data += std::string(msg, bytesReceivedMsg);
        }
    }


    //std::cout << "Message from user: " << _client->data << "\n Enough users have logged in\n";

    std::string response = "Thank you for logging into the system user" + std::to_string(_client->number) +
            " you've sent " + std::to_string(size) + " bytes\n";

    std::unique_lock bufferLock(bufferMutex);

    bufferCV.wait(bufferLock,[]{return bufferInUse;});
    bufferInUse = true;
    log << _client->data << "\n";;
    bufferInUse = false;
    std::this_thread::sleep_for(std::chrono::milliseconds (10));
    bufferCV.notify_all();
    auto sendCode = send(_client->clientSocket, response.c_str(), BUFFER_SIZE, 0);
    std::cout << "User " << _client->number << " proccesed\n";

    closesocket(clientSocket);
    delete _client;
    users--;
    userCountCV.notify_all();
}

void serverThread(){
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
        return;
    }


    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Can't create socket! Quitting" << std::endl;
        WSACleanup();
        return;
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

    std::vector<std::thread> clients;

    std::ofstream log;
    log.open(("..\\log.txt"));

    while(true){
        SOCKET ClientSocket = accept(listening,NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
            closesocket(listening);
            WSACleanup();
            return;
        }

        clients.emplace_back(handleClients, ClientSocket, std::ref(log));
        clients.back().detach();

        std::cout << "Adding new thread...\n";
    }

    log.close();

    closesocket(listening);
    WSACleanup();
}

int main() {
    std::thread server(serverThread);
    server.join();
}