#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>
#include <chrono>

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
int encrypt(int message, int publicKey, int n) {
    return std::fmod(std::pow(message, publicKey), n);
}

// Funkcja do deszyfrowania wiadomości
int decrypt(int encryptedMessage, int privateKey, int n) {
    return std::fmod(std::pow(encryptedMessage, privateKey), n);
}

double measureEncryptionTime(int message, int publicKey, int n) {
    auto start = std::chrono::steady_clock::now();
    encrypt(message, publicKey, n);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int maino() {
    // Dane testowe
    int p = 61; // Pierwsza liczba pierwsza
    int q = 53; // Druga liczba pierwsza

    // Klucze
    int publicKey, privateKey;
    generateKeys(p, q, publicKey, privateKey);
    int n = p * q; // Moduł n

    // Wiadomość do zaszyfrowania
    int message = 1234567890;

    // Szyfrowanie wiadomości
    int encryptedMessage = encrypt(message, publicKey, n);

    // Deszyfrowanie wiadomości
    int decryptedMessage = decrypt(encryptedMessage, privateKey, n);

    // Wyświetlanie wyników
    std::cout << "Public Key: " << publicKey << std::endl;
    std::cout << "Private Key: " << privateKey << std::endl;
    std::cout << "Original Message: " << message << std::endl;
    std::cout << "Encrypted Message: " << encryptedMessage << std::endl;
    std::cout << "Decrypted Message: " << decryptedMessage << std::endl;
    std::cout << "Encryption Time: " << measureEncryptionTime(message, publicKey, n) << " ms" << std::endl;
    return 0;
}
