cmake_minimum_required(VERSION 3.20)
project(AVL C)

add_library(avl avl.c)

add_executable(main main.c)
target_link_libraries(main PRIVATE avl)