cmake_minimum_required(VERSION 3.23)
project(Lab1)

set(CMAKE_CXX_STANDARD 23)

set(LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libraries")

include_directories(${LIBRARY_DIR})

set(LIB_ONE_NAME LibOne)
set(LIB_TWO_NAME LibOne)

set(LIBRARY_ONE_PATH "${LIBRARY_DIR}/build/Debug/${LIB_ONE_NAME}.dll")
set(LIBRARY_TWO_PATH "${LIBRARY_DIR}/build/Debug/${LIB_TWO_NAME}.dll")

# Sprawdź, czy plik biblioteki istnieje
if (NOT EXISTS ${LIBRARY_ONE_PATH})
    message(FATAL_ERROR "Biblioteka ${LIBRARY_PATH} nie istnieje.")
endif()

if (NOT EXISTS ${LIBRARY_TWO_PATH})
    message(FATAL_ERROR "Biblioteka ${LIBRARY_PATH} nie istnieje.")
endif()

# Dodaj plik wykonywalny Lab1
add_executable(Lab1 main.cpp)

# Linkuj bibliotekę dynamiczną z projektem Lab1
target_link_libraries(Lab1 ${LIBRARY_ONE_PATH})