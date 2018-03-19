cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(func_adress func_adress.cpp ElfHandler.cpp)

install(TARGETS func_adress DESTINATION bin/elftools)
