cmake_minimum_required(VERSION 3.15)
project(filewriter)

set(CMAKE_CXX_STANDARD 14)
find_package(GTest REQUIRED)

include_directories(
        ${GTEST_INCLUDE_DIRS}
)
add_executable(filewriter main.cpp FileWriter.cpp FileWriter.h Record.cpp Record.h)
target_link_libraries(filewriter ${GTEST_LIBRARIES} )
enable_testing()
