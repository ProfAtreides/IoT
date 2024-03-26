//
// Created by dambr on 26.03.2024.
//

#include <cstring>

#ifndef LAB1_LIB_TWO_HPP
#define IOT_LIB_TWO_HPP __declspec(dllexport)
#else
#define IOT_LIB_TWO_HPP __declspec(dllimport)
#endif LAB1_LIB_TWO_HPP

extern "C" IOT_LIB_TWO_HPP char* printText(char* s)
{
    const char* prefix = "I care about arguments ";
    int lenPrefix = strlen(prefix);
    int lenArg = strlen(s);

    int totalLength = lenPrefix + lenArg;

    char* result = new char[totalLength + 1];

    strcpy(result, prefix);
    strcpy(result + lenPrefix, s);

    return result;
}



