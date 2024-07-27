cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

project(cv-snake)

add_executable(cv-snake src/game.cpp)

add_subdirectory(libs/raylib)

include_directories(include libs/raygui/src)

target_link_libraries(cv-snake raylib)