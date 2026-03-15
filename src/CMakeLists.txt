cmake_minimum_required(VERSION 3.20)
project(CSV C)

add_library(csv csv.c)

add_executable(main main.c)
target_link_libraries(main PRIVATE csv)