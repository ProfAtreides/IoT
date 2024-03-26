#include <iostream>
#include <windows.h>

using namespace std;

int main() {

    auto libOneAddress = LoadLibrary(TEXT("LibOne.dll"));
    if (!libOneAddress)
    {
        cerr << "Library one failed to load\n";
        return 1;
    }

    auto libTwoAddress = LoadLibrary(TEXT("LibTwo.dll"));
    if (!libTwoAddress) {
        cerr << "Library two failed to load\n";
        return 1;
    }

    typedef char *(*Text)(char*);

    auto textFunction = reinterpret_cast<Text>(GetProcAddress(libOneAddress, "printText"));
    if (!textFunction) {
        std::cerr << "printText doesn't exist in LibOne\n";
        return 1;
    }

    cout << textFunction(" I want to test the function") << "\n";

    textFunction = reinterpret_cast<Text>(GetProcAddress(libTwoAddress, "printText"));
    if (!textFunction) {
        std::cerr << "printText doesn't exist in LibTwo\n";
        return 1;
    }

    cout << textFunction((" I want to test the function")) << "\n";

    FreeLibrary(libOneAddress);
    FreeLibrary(libTwoAddress);

    return 0;
}
