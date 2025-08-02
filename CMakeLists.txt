cmake_minimum_required(VERSION 3.30)
project(datastructures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-4") # valgrind
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")        # gdb

include(FetchContent)
FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.9.0
)
FetchContent_MakeAvailable(catch2)

add_library(j_range_basics)
target_sources(j_range_basics
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/range_basics.cppm
)

add_library(j_array)
target_sources(j_array
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/Array/array.cppm
)

add_executable(test_array
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/test_array.cpp
)
target_link_libraries(test_array PRIVATE j_array Catch2::Catch2WithMain)

add_library(j_list)
target_sources(j_list
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/LinkedList/list.cppm
)
target_link_libraries(j_list PRIVATE j_range_basics)

add_executable(test_list
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/test_list.cpp
)
target_link_libraries(test_list PRIVATE j_list Catch2::Catch2WithMain)

add_executable(bench_list
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/bench_list.cpp
)
target_link_libraries(bench_list PRIVATE j_list Catch2::Catch2WithMain)

add_library(j_forward_list)
target_sources(j_forward_list
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/LinkedList/forward_list.cppm
)
target_link_libraries(j_forward_list PRIVATE j_range_basics)

add_executable(test_forward_list
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/test_forward_list.cpp
)
target_link_libraries(test_forward_list PRIVATE j_forward_list Catch2::Catch2WithMain)

add_executable(bench_forward_list
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/bench_forward_list.cpp
)
target_link_libraries(bench_forward_list PRIVATE j_forward_list Catch2::Catch2WithMain)