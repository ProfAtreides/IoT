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
        result += (char)data[i];
    }
    return result;
}

std::vector<int> strToVec(std::string str){
    std::vector<int> result;

    for(int i =0 ;i < str.length();i++) {
        result.push_back(str[i] - '0');
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
    while(true){

        SOCKET sock  = socket(AF_INET, SOCK_STREAM, 0);

        if (sock == INVALID_SOCKET) {
            std::cerr << "Can't create sock! Quitting" << std::endl;
            return 1;
        }

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

        // Receive data from client
        std::string message;

         std::cin >> message;

        int p = 17; // Pierwsza liczba pierwsza
        int q = 19; // Druga liczba pierwsza

        int publicKey, privateKey;
        generateKeys(p, q, publicKey, privateKey);
        int n = p * q; // Moduł n

        std::vector<int> encryptedMessage = encrypt(message, publicKey, n);

        auto temp = vecToStr(encryptedMessage);

        int bytesSent = (send(sock, vecToStr(encryptedMessage).c_str(),
                              encryptedMessage.size(), 0));

        char *result;

        int bytesRecv = (recv(sock, result, 4096, 0));
        // Close client sock

        std::cout << decrypt(strToVec(result),privateKey,n);
        closesocket(sock);
     }



}
