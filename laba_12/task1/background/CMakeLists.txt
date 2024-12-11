# Задание: Создать библиотеку из background.c и слинковать ее с библиотекой pipes
cmake_minimum_required(VERSION 3.10)	  

project(hello_cmake)			

set(SOURCE_EXE main.c)		
set(SOURCE_LIB functions.c)			

add_library(bg_lib STATIC ${SOURCE_LIB})	

add_executable(main ${SOURCE_EXE})	

target_link_libraries(pipes bg_lib)	