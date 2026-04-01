cmake_minimum_required(VERSION 3.20)
project(STATES C)

add_library(states states.c)

add_executable(main main.c)
target_link_libraries(main PRIVATE states)