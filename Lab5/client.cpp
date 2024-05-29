#include <iostream>
#include <bitset>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>

using namespace std;

// Pomocnicze funkcje do konwersji i wyświetlania
string binToStr(const bitset<64>& bits) {
    stringstream ss;
    for (int i = 0; i < 64; i += 8) {
        bitset<8> byte;
        for (int j = 0; j < 8; j++) {
            byte[j] = bits[i + j];
        }
        ss << char(byte.to_ulong());
    }
    return ss.str();
}

bitset<64> strToBin(const string& str) {
    bitset<64> bits;
    for (int i = 0; i < 8; i++) {
        bitset<8> byte(str[i]);
        for (int j = 0; j < 8; j++) {
            bits[i * 8 + j] = byte[j];
        }
    }
    return bits;
}

// Prosta implementacja permutacji
bitset<64> permute(const bitset<64>& input, const vector<int>& table) {
    bitset<64> output;
    for (size_t i = 0; i < table.size(); i++) {
        output[63 - i] = input[64 - table[i]];
    }
    return output;
}

// Placeholdery dla permutacji i kluczy
const vector<int> initial_permutation = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
};

const vector<int> final_permutation = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
};

// Funkcja podstawienia dla DES (mock)
bitset<32> f_function(const bitset<32>& R, const bitset<48>& K) {
    // Placeholder - rzeczywista implementacja wymaga S-boxów i permutacji
    return R;
}

// Szyfrowanie jednego bloku DES
bitset<64> des_encrypt_block(const bitset<64>& plaintext, const bitset<64>& key) {
    // Początkowa permutacja
    bitset<64> permuted_text = permute(plaintext, initial_permutation);

    // Podział na L i R
    bitset<32> L = permuted_text.to_ullong() >> 32;
    bitset<32> R = permuted_text.to_ullong();

    // Rounds (mock example with only one round for simplicity)
    bitset<48> K = key.to_ullong() >> 16; // Simplified key schedule
    bitset<32> newR = L ^ f_function(R, K);
    L = R;
    R = newR;

    // Połączenie L i R
    bitset<64> preoutput = (bitset<64>(L.to_ulong()) << 32) | bitset<64>(R.to_ulong());

    // Końcowa permutacja
    bitset<64> ciphertext = permute(preoutput, final_permutation);
    return ciphertext;
}

// Deszyfrowanie jednego bloku DES (z uproszczoną logiką)
bitset<64> des_decrypt_block(const bitset<64>& ciphertext, const bitset<64>& key) {
    // Początkowa permutacja
    bitset<64> permuted_text = permute(ciphertext, initial_permutation);

    // Podział na L i R
    bitset<32> L = permuted_text.to_ullong() >> 32;
    bitset<32> R = permuted_text.to_ullong();

    // Rounds (mock example with only one round for simplicity)
    bitset<48> K = key.to_ullong() >> 16; // Simplified key schedule
    bitset<32> newL = R ^ f_function(L, K);
    R = L;
    L = newL;

    // Połączenie L i R
    bitset<64> preoutput = (bitset<64>(L.to_ulong()) << 32) | bitset<64>(R.to_ulong());

    // Końcowa permutacja
    bitset<64> plaintext = permute(preoutput, final_permutation);
    return plaintext;
}

int main() {
    // Klucz symetryczny (64 bity, dla uproszczenia wygenerowany statycznie)
    bitset<64> des_key("1010101010111011000010010001100000100111001101101100110011011101");

    // Przykładowy tekst do zaszyfrowania
    string plaintext = "fnasjf@1"; // 8 znaków = 64 bity
    bitset<64> plaintext_bits = strToBin(plaintext);
    cout << "Text: " << binToStr(plaintext_bits) << endl;

    // Szyfrowanie
    bitset<64> ciphertext_bits = des_encrypt_block(plaintext_bits, des_key);
    cout << "Decrypted text: " << binToStr(ciphertext_bits) << endl;

    // Deszyfrowanie
    bitset<64> decrypted_bits = des_decrypt_block(ciphertext_bits, des_key);
    cout << "Encrypted text: " << binToStr(decrypted_bits) << endl;

    return 0;
}
