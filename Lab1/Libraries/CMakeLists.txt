cmake_minimum_required(VERSION 3.0)

set(HEADER_ONE LibOne.hpp)
set(SOURCE_ONE LibOne.cpp)

set(HEADER_TWO LibTwo.hpp)
set(SOURCE_TWO LibTwo.cpp)

if (WIN32)
    add_definitions(-DEXAMPLEINTERFACE1_EXPORTS)
endif()

add_library(LibOne SHARED ${SOURCE_ONE} ${HEADER_ONE})
add_library(LibTwo SHARED ${SOURCE_TWO} ${HEADER_TWO})

set_target_properties(LibOne PROPERTIES
        VERSION 1.0
        SOVERSION 1
        )
set_target_properties(LibTwo PROPERTIES
        VERSION 1.0
        SOVERSION 1
        )

set_target_properties(LibOne PROPERTIES
        OUTPUT_NAME "LibOne"
        )
set_target_properties(LibTwo PROPERTIES
        OUTPUT_NAME "LibTwo"
        )

# Ustaw katalog, w którym biblioteka zostanie zainstalowana
install(TARGETS LibOne LibTwo
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
        )

message(${CMAKE_BINARY_DIR})

add_custom_command(TARGET LibOne LibTwo
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:LibOne>
        $<TARGET_FILE:LibTwo>
        ${CMAKE_BINARY_DIR}/../../cmake-build-debug/
        COMMENT "Copying DLLs to output folder"
        )