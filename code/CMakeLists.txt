cmake_minimum_required(VERSION 2.8.4)
project(code)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/board.cpp
    src/board.h
    src/main.cpp
    src/renderer.cpp
    src/renderer.h
    src/sound.cpp
    src/sound.h)

add_executable(code ${SOURCE_FILES})