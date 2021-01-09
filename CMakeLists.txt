cmake_minimum_required(VERSION 3.0.0)
project(FTP_Server VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(FTP_Server main.cpp main.h Commands.cpp Functions.cpp INI_Parser.cpp INI_Parser.h Login.cpp Script.h Socket.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
