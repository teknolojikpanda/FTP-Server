cmake_minimum_required(VERSION 3.16)
project(FTP_Server)

set(CMAKE_CXX_STANDARD 11)

add_executable(FTP_Server main.cpp main.h Socket.cpp Commands.cpp Login.cpp Script.h INI_Parser.cpp INI_Parser.h Functions.cpp)