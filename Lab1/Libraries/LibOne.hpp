//
// Created by dambr on 26.03.2024.
//

#ifndef LAB1_LIBONE_HPP
#define IOT_LIBONE_HPP __declspec(dllexport)
#else
#define IOT_LIBONE_HPP __declspec(dllimport)
#endif LAB1_LIBONE_HPP

extern "C" IOT_LIBONE_HPP char* printText(char* s)
{
    return "I don't care about arguments";
}



