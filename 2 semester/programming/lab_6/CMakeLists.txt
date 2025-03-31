cmake_minimum_required(VERSION 3.10)
project(lab10)

add_library(lab10 laba_10_other.c laba_10_header.h)

add_executable(my_program laba_10_main.c)

target_link_libraries(my_program lab10)
