# Задание: Создать библиотеку pipes из read_pipe.c write_pipe.c
cmake_minimum_required(VERSION 3.10)	  

project(hello_cmake)			
add_library(pipec STATIC ${SOURCE_LIB})	
add_executable(main ${SOURCE_EXE})	
target_link_libraries(main bg_lib)	