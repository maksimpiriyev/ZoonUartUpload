# cmake_minimum_required(VERSION <specify CMake version here>)
project(ZoonUartUpload)

#set(CMAKE_CXX_STANDARD 14)

add_executable(ZoonUartUpload main.cpp serial_port.cpp)