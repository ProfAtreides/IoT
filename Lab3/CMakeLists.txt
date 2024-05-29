cmake_minimum_required(VERSION 3.27)
project(Lab2)

set(CMAKE_CXX_STANDARD 23)

add_executable(LAB3_SERVER server.cpp)
add_executable(LAB3_CLIENT client.cpp)

target_link_libraries(LAB3_SERVER PRIVATE ws2_32)
target_link_libraries(LAB3_CLIENT PRIVATE ws2_32)
