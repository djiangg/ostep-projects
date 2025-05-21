cmake_minimum_required(VERSION 3.28)
project(initial-utilities C)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_C_STANDARD 23)

add_subdirectory(wcat)
add_subdirectory(wgrep)
add_subdirectory(wzip)
add_subdirectory(wunzip)
