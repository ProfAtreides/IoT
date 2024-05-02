cmake_minimum_required(VERSION 3.27)
project(IoT)

set(CMAKE_CXX_STANDARD 23)

add_executable(LAB4_SRVER server.cpp)
add_executable(LAB4_CLIENT client.cpp)

target_link_libraries(LAB4_SRVER PRIVATE ws2_32)
target_link_libraries(LAB4_CLIENT PRIVATE ws2_32)
