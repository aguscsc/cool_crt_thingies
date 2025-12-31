cmake_minimum_required(VERSION 3.10)

project(oscilloscope)

find_package(SDL2 OpenGL GLEW)

add_executable(oscilloscope main.cpp)


