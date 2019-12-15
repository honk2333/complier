cmake_minimum_required(VERSION 3.14)
project(Clion)

set(CMAKE_CXX_STANDARD 14)

add_executable(Clion pre_grammer.cpp token.cpp synbl.cpp grammer.cpp complier.cpp language.cpp)