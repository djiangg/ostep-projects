cmake_minimum_required(VERSION 3.28)
project(initial_kv C)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_C_STANDARD 23)

add_executable(kv kv.c list_node.c)
set_target_properties(kv PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
