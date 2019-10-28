cmake_minimum_required(VERSION 3.15)
project(filewriter)

set(CMAKE_CXX_STANDARD 14)

add_executable(filewriter main.cpp FileWriter.cpp FileWriter.h Record.cpp Record.h)