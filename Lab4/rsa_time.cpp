#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>

// Funkcja do mierzenia czasu szyfrowania
double measureEncryptionTime(int message, int publicKey, int n) {
    auto start = std::chrono::steady_clock::now();
    encrypt(message, publicKey, n);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    // Dane testowe
    int p = 61; // Pierwsza liczba pierwsza
    int q = 53; // Druga liczba pierwsza

    // Klucze
    int publicKey, privateKey;
    generateKeys(p, q, publicKey, privateKey);
    int n = p * q; // Moduł n

    // Rozmiary pakietów do testowania
    int packageSizes[] = {64, 128, 256, 512};

    // Mierzenie czasu szyfrowania dla różnych rozmiarów pakietów
    for (int packageSize : packageSizes) {
        // Tworzenie wiadomości o odpowiednim rozmiarze
        int message = 0;
        for (int i = 0; i < packageSize / 4; ++i) {
            message = message * 10000 + rand() % 10000;
        }

        // Mierzenie czasu szyfrowania
        double encryptionTime = measureEncryptionTime(message, publicKey, n);

        // Wyświetlanie wyników
        std::cout << "Package Size: " << packageSize << " bytes" << std::endl;
        std::cout << "Encryption Time: " << encryptionTime << " ms" << std::endl;
    }

    return 0;
}
