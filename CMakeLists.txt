cmake_minimum_required (VERSION 2.6)
project (elftools)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(func_adress func_adress.cpp ElfFile.cpp)
add_executable(read_arch read_arch.cpp ElfFile.cpp)

install(TARGETS func_adress DESTINATION bin/elftools)
install(TARGETS read_arch DESTINATION bin/elftools)
