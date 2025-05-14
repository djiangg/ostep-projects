cmake_minimum_required(VERSION 3.28)
project(initial_reverse C)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_C_STANDARD 23)

add_executable(reverse reverse.c list_node.c)
set_target_properties(reverse PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
