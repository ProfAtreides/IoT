#include <iostream>
#include <string>
#include <WS2tcpip.h> // Winsock library
#include <chrono>
#include <vector>
#include <cmath>
#pragma comment (lib, "ws2_32.lib") // Winsock library

// Implementacja algorytmu Euklidesa dla znajdowania największego wspólnego dzielnika (NWD)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Funkcja do obliczania odwrotności modulo (a^(-1) mod m)
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return 1;
}

// Funkcja do generowania kluczy publicznego i prywatnego
void generateKeys(int p, int q, int& publicKey, int& privateKey) {
    int n = p * q; // Obliczanie modułu n
    int phi = (p - 1) * (q - 1); // Obliczanie funkcji Eulera phi(n)

    // Wybór liczby e względnie pierwszej z phi
    int e = 2;
    while (e < phi) {
        if (gcd(e, phi) == 1)
            break;
        else
            e++;
    }

    // Obliczanie klucza prywatnego (d)
    int d = modInverse(e, phi);

    publicKey = e;
    privateKey = d;
}

// Funkcja do szyfrowania wiadomości
std::vector<int> encrypt(const std::string& message, int publicKey, int n) {
    std::vector<int> encryptedMessage;
    for (char c : message) {
        int asciiVal = static_cast<int>(c);
        encryptedMessage.push_back(std::fmod(std::pow(asciiVal, publicKey), n));
    }
    return encryptedMessage;
}

// Function to decrypt a message
std::string decrypt(const std::vector<int>& encryptedMessage, int privateKey, int n) {
    std::string decryptedMessage;
    for (int val : encryptedMessage) {
        char decryptedChar = static_cast<char>(std::fmod(std::pow(val, privateKey), n));
        decryptedMessage.push_back(decryptedChar);
    }
    return decryptedMessage;
}

double measureEncryptionTime(std::string message, int publicKey, int n) {
    auto start = std::chrono::steady_clock::now();
    encrypt(message, publicKey, n);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

std::string vecToStr(std::vector<int> data)
{
    std::string result;
    for(int i = 0; i < data.size(); i++)
    {
        result += std::to_string(data[i]);
    }
    return result;
}

int main() {
    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
        return 1;
    }

    // Create socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Can't create socket! Quitting" << std::endl;
        WSACleanup();
        return 1;
    }

    std::cout <<"Creating server socket..\n";

    // Bind the socket to IP and port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    while (true) {
        // Wait for a connection
        sockaddr_in client;
        int clientSize = sizeof(client);

        SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Can't accept client connection! Quitting" << std::endl;
            closesocket(listening);
            WSACleanup();
            return 1;
        }

        // Receive data from client
        std::string data;
        recv(clientSocket, (char*)&data, sizeof(int), 0);

        int p = 17; // Pierwsza liczba pierwsza
        int q = 19; // Druga liczba pierwsza

        // Klucze
        int publicKey, privateKey;
        generateKeys(p, q, publicKey, privateKey);
        int n = p * q; // Moduł n

        std::vector<int> encryptedMessage = encrypt(data, publicKey, n);

        std::cout << "Keys have been generated. Public key: " << publicKey << ", Private key: " << privateKey << std::endl;
        std::cout << "Original message: " << data << " encrypted data: "<< vecToStr(encryptedMessage) << " Time spent: " << measureEncryptionTime(data,publicKey,n)<<" ms" << std::endl;
        // Send result to client
        send(clientSocket, (char*)nullptr, sizeof(double), 0);

        // Close client socket
        closesocket(clientSocket);
    }
}