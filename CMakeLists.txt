cmake_minimum_required(VERSION 4.0)
project(lab6_coroutine_guess_number)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab6_coroutine_guess_number
        main.cpp
        coroutines.cpp
        guess_game.cpp)
