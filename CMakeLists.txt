cmake_minimum_required(VERSION 3.28)
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

include(CTest)

# file(GLOB_RECURSE MODULE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/modules/*.cppm")
# FILES -> ${MODULE_FILES}

add_library(j STATIC)
target_sources(j
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES
        BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/modules
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/j.cppm
        # ${CMAKE_CURRENT_SOURCE_DIR}/modules/algorithms/algorithm.cppm
        # ${CMAKE_CURRENT_SOURCE_DIR}/modules/algorithms/heap_algo.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/Array/array.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/LinkedList/list.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/LinkedList/forward_list.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/Vector/vector.cppm
        # ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/Deque/deque.cppm
        # ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/Deque/stack.cppm
        # ${CMAKE_CURRENT_SOURCE_DIR}/modules/datastructures/Deque/queue.cppm
)

# --------------- Add Tests and Benchmarks ---------------
add_executable(test_array
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/test_array.cpp
)
target_link_libraries(test_array PRIVATE j Catch2::Catch2WithMain)

add_executable(bench_array
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/benchmark/bench_array.cpp
)
target_link_libraries(bench_array PRIVATE j Catch2::Catch2WithMain)

add_executable(test_list
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/test_list.cpp
)
target_link_libraries(test_list PRIVATE j Catch2::Catch2WithMain)

add_executable(bench_list
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/benchmark/bench_list.cpp
)
target_link_libraries(bench_list PRIVATE j Catch2::Catch2WithMain)

add_executable(test_forward_list
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/test_forward_list.cpp
)
target_link_libraries(test_forward_list PRIVATE j Catch2::Catch2WithMain)

add_executable(bench_forward_list
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/benchmark/bench_forward_list.cpp
)
target_link_libraries(bench_forward_list PRIVATE j Catch2::Catch2WithMain)

add_executable(test_vector
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/test_vector.cpp
)
target_link_libraries(test_vector PRIVATE j Catch2::Catch2WithMain)

add_executable(bench_vector
        ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/benchmark/bench_vector.cpp
)
target_link_libraries(bench_vector PRIVATE j Catch2::Catch2WithMain)

# add_executable(test_deque
#         ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/test_deque.cpp
# )
# target_link_libraries(test_deque PRIVATE j Catch2::Catch2WithMain)

# add_executable(bench_deque
#         ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/benchmark/bench_deque.cpp
# )
# target_link_libraries(bench_deque PRIVATE j Catch2::Catch2WithMain)

# add_executable(test_stack
#         ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/test_stack.cpp
# )
# target_link_libraries(test_stack PRIVATE j Catch2::Catch2WithMain)
#
# add_executable(bench_stack
#         ${CMAKE_CURRENT_SOURCE_DIR}/test/datastructures/benchmark/bench_stack.cpp
# )
# target_link_libraries(bench_stack PRIVATE j Catch2::Catch2WithMain)

add_executable(main
        ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
)
target_link_libraries(main PRIVATE j)

# --------------- Add CTest ---------------
enable_testing()

set(CTEST_MEMORYCHECK_COMMAND "/usr/bin/valgrindc")
set(CTEST_MEMORYCHECK_COMMAND_OPTIONS
    "--leak-check=full;--show-leak-kinds=all;--error-exitcode=1;--track-origins=yes"
)

add_test(NAME test_array COMMAND test_array)
add_test(NAME test_list COMMAND test_list)
add_test(NAME test_forward_list COMMAND test_forward_list)
add_test(NAME test_vector COMMAND test_vector)
# add_test(NAME test_deque COMMAND test_deque)
