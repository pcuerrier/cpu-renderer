cmake_minimum_required(VERSION 3.20.0)
project(cpu-renderer VERSION 0.0.1 LANGUAGES CXX C)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(src)

add_subdirectory(lib/SDL)
add_subdirectory(lib/googletest)
add_subdirectory(src)
add_subdirectory(tests)
