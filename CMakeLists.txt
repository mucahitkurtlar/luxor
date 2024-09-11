cmake_minimum_required(VERSION 3.10)
project(Luxor C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(luxor src/main.c)

find_library(CRYPTSETUP_LIB cryptsetup)

if(CRYPTSETUP_LIB)
    target_link_libraries(luxor PRIVATE ${CRYPTSETUP_LIB})
else()
    message(FATAL_ERROR "cryptsetup library not found")
endif()

set_target_properties(luxor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
