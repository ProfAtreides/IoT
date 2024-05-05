cmake_minimum_required(VERSION 3.27)
project(Lab2)

set(CMAKE_CXX_STANDARD 23)

add_executable(LAB2_SERVER server.cpp)
add_executable(LAB2_CLIENT client.cpp)

target_link_libraries(LAB2_SERVER PRIVATE ws2_32)
target_link_libraries(LAB2_CLIENT PRIVATE ws2_32)
