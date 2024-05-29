#include <iostream>
#include <string>
#include <cmath>
#include <bitset>

using namespace std;

unsigned long long powerMod(unsigned long long a, unsigned long long b, unsigned long long p) {
    unsigned long long res = 1;
    a = a % p;
    while (b > 0) {
        if (b % 2 == 1) {
            res = (res * a) % p;
        }
        b = b >> 1;
        a = (a * a) % p;
    }
    return res;
}

int main()
{
    // Ustalanie wartości publicznych
    unsigned long long p = 317; // Duża liczba pierwsza
    unsigned long long g = 473535957707;  // Pierwiastek pierwotny modulo p

    // Wartości prywatne (wybierane losowo)
    srand(time(0));
    unsigned long long a = rand() % (p - 1) + 1; // Klucz prywatny Alicji
    unsigned long long b = rand() % (p - 1) + 1; // Klucz prywatny Boba

    // Generowanie wartości publicznych
    unsigned long long A = powerMod(g, a, p); // A = g^a % p
    unsigned long long B = powerMod(g, b, p); // B = g^b % p

    // Wymiana kluczy
    std::cout << "Wartość publiczna Alicji (A): " << A << std::endl;
    std::cout << "Wartość publiczna Boba (B): " << B << std::endl;

    // Obliczanie wspólnego klucza
    unsigned long long keyA = powerMod(B, a, p); // Klucz wspólny obliczony przez Alicję
    unsigned long long keyB = powerMod(A, b, p); // Klucz wspólny obliczony przez Boba

    // Sprawdzenie, czy klucze są takie same
    if (keyA == keyB) {
        std::cout << "Klucz wspólny został uzgodniony pomyślnie: " << keyA << std::endl;
    } else {
        std::cout << "Błąd: klucze nie są takie same!" << std::endl;
    }

    return 0;
}