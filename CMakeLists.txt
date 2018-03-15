cmake_minimum_required (VERSION 2.6)

add_executable(func_adress func_adress.c)

install(TARGETS func_adress DESTINATION bin/elftools)
