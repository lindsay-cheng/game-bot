cmake_minimum_required(VERSION 3.16)
project(reversi C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(reversi
    src/main.c
    src/reversi_board.c
    src/reversi_ai.c
)

target_include_directories(reversi PRIVATE include)

if(NOT APPLE)
    target_link_libraries(reversi m)
endif()
