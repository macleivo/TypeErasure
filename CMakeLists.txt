cmake_minimum_required(VERSION 3.5)

project(TypeErasure LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TypeErasure main.cpp shape.h)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
