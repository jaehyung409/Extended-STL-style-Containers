/*
 * @ Created by jaehyung409 on 25. 7. 28.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN
import j.array;

#include <catch2/catch_all.hpp>
#include <array>
#include <algorithm>

TEST_CASE("Array Basic") {
    j::array<int, 5> arr = {1, 2, 3, 4, 5};
    j::array arr_deduction = {'1', '2', '3', '4', '5'}; // Deduction guide usage
    j::array<int, 0> empty_arr; // Empty array

    SECTION("Accessing elements, operator[]") {
        REQUIRE(arr[0] == 1);
        REQUIRE(arr[1] == 2);
        REQUIRE(arr[2] == 3);
        REQUIRE(arr[3] == 4);
        REQUIRE(arr[4] == 5);
    }

    SECTION("Accessing elements, at()") {
        REQUIRE(arr.at(0) == 1);
        REQUIRE(arr.at(1) == 2);
        REQUIRE(arr.at(2) == 3);
        REQUIRE(arr.at(3) == 4);
        REQUIRE(arr.at(4) == 5);

        // Test out of range
        REQUIRE_THROWS_AS(arr.at(5), std::out_of_range);
    }

    SECTION("Accessing elements, front() and back()") {
        REQUIRE(arr.front() == 1);
        REQUIRE(arr.back() == 5);
        REQUIRE(arr_deduction.front() == '1');
        REQUIRE(arr_deduction.back() == '5');
    }

    SECTION("Data pointer") {
        int* data_ptr = arr.data();
        REQUIRE(data_ptr[0] == 1);
        REQUIRE(data_ptr[4] == 5);
    }

    SECTION("Size and capacity") {
        REQUIRE(arr.size() == 5);
        REQUIRE(arr.max_size() == 5);
        REQUIRE(!arr.empty());
        REQUIRE(empty_arr.size() == 0);
        REQUIRE(empty_arr.max_size() == 0);
        REQUIRE(empty_arr.empty());
    }

    SECTION("Fill method") {
        j::array<int, 2> fill_arr;
        fill_arr.fill(10);
        for (auto num : fill_arr) {
            REQUIRE(num == 10);
        }
    }

    SECTION("Iterators & Reverse Iterators") {
        auto it = arr.begin();
        REQUIRE(*it == 1);
        ++it;
        REQUIRE(*it == 2);
        --it;
        REQUIRE(*it == 1);

        auto rit = arr.rbegin();
        REQUIRE(*rit == 5);
        ++rit;
        REQUIRE(*rit == 4);
        --rit;
        REQUIRE(*rit == 5);

        auto const_it = arr.cbegin();
        REQUIRE(*const_it == 1);
        ++const_it;
        REQUIRE(*const_it == 2);
    }

    SECTION("Swap") {
        j::array<int, 5> arr2 = {6, 7, 8, 9, 10};
        arr.swap(arr2);
        REQUIRE(arr[0] == 6);
        REQUIRE(arr2[0] == 1);
        swap(arr, arr2);
        REQUIRE(arr[0] == 1);
        REQUIRE(arr2[0] == 6);
    }

    SECTION("Non-member get()") {
        REQUIRE(j::get<0>(arr) == 1);
        REQUIRE(j::get<1>(arr) == 2);
        REQUIRE(j::get<4>(arr) == 5);
        REQUIRE(j::get<0>(arr_deduction) == '1');
        REQUIRE(j::get<4>(arr_deduction) == '5');
    }

    SECTION("Non-member to_array()") {
        int arr_c[5] = {1, 2, 3, 4, 5};
        auto converted_arr = j::to_array(arr_c);
        REQUIRE(converted_arr.size() == 5);
        REQUIRE(converted_arr[0] == 1);
        REQUIRE(converted_arr[4] == 5);
    }

    SECTION("Comparison operators") {
        j::array<int, 5> arr1 = {1, 2, 3, 4, 5};
        j::array<int, 5> arr2 = {1, 2, 3, 4, 5};
        j::array<int, 5> arr3 = {5, 4, 3, 2, 1};
        j::array<int, 5> arr4 = {1, 2, 3, 4, 6};

        REQUIRE(arr1 == arr2);
        REQUIRE(arr1 != arr3);
        REQUIRE(arr1 != arr4);

        REQUIRE((arr1 <=> arr2) == std::strong_ordering::equal);
        REQUIRE((arr1 <=> arr3) == std::strong_ordering::less);
        REQUIRE((arr3 <=> arr1) == std::strong_ordering::greater);
        REQUIRE((arr1 <=> arr4) == std::strong_ordering::less);
    }
}

TEST_CASE("Array Edge Cases") {
    j::array<int, 0> empty_arr;
    j::array<int, 1> single_element_arr = {42};

    SECTION("Empty array properties") {
        REQUIRE(empty_arr.size() == 0);
        REQUIRE(empty_arr.max_size() == 0);
        REQUIRE(empty_arr.empty());
    }

    SECTION("Single element array properties") {
        REQUIRE(single_element_arr.size() == 1);
        REQUIRE(single_element_arr.max_size() == 1);
        REQUIRE(!single_element_arr.empty());
        REQUIRE(single_element_arr.front() == 42);
        REQUIRE(single_element_arr.back() == 42);
    }
}

constexpr size_t N = 500'000;

TEST_CASE("Array Benchmarks: fill") {
    SECTION("j::array fill") {
        j::array<int, N> arr;
        BENCHMARK("Fill j::array") {
            arr.fill(369);
            return arr.front() + arr.back() + arr[N / 3];
        };
    }
    SECTION("std::array fill") {
        std::array<int, N> arr;
        BENCHMARK("Fill std::array") {
            arr.fill(369);
            return arr.front() + arr.back() + arr[N / 3];
        };
    }
}

TEST_CASE("Array Benchmarks: with std::sort") {
    SECTION("std::sort with j::array") {
        j::array<int, N> arr;
        for (size_t i = 0; i < N; ++i) {
            arr[i] = N - i; // Reverse order
        }
        BENCHMARK("Sort j::array") {
            std::sort(arr.begin(), arr.end());
            return arr[N / 2]; // Return middle element after sort
        };
    }
    SECTION("std::sort with std::array") {
        std::array<int, N> arr;
        for (size_t i = 0; i < N; ++i) {
            arr[i] = N - i; // Reverse order
        }
        BENCHMARK("Sort std::array") {
            std::sort(arr.begin(), arr.end());
            return arr[N / 2]; // Return middle element after sort
        };
    }
}

TEST_CASE("Array Benchmarks: with std::copy") {
    SECTION("std::copy with j::array") {
        j::array<int, N> arr_from;
        j::array<int, N> arr_to;
        for (size_t i = 0; i < N; ++i) {
            arr_from[i] = i; // Ascending order
        }
        BENCHMARK("Copy j::array") {
            std::copy(arr_from.begin(), arr_from.end(), arr_to.begin());
            return arr_to.front() + arr_to.back() + arr_to[N / 3];
        };
    }
    SECTION("std::copy with std::array") {
        std::array<int, N> arr_from;
        std::array<int, N> arr_to;
        for (size_t i = 0; i < N; ++i) {
            arr_from[i] = i; // Ascending order
        }
        BENCHMARK("Copy std::array") {
            std::copy(arr_from.begin(), arr_from.end(), arr_to.begin());
            return arr_to.front() + arr_to.back() + arr_to[N / 3];
        };
    }
}

TEST_CASE("Array Benchmarks: swap") {
    SECTION("Swap j::array") {
        j::array<int, N> arr1;
        j::array<int, N> arr2;
        for (size_t i = 0; i < N; ++i) {
            arr1[i] = i; // Ascending order
            arr2[i] = N - i; // Descending order
        }
        BENCHMARK("Swap j::array") {
            swap(arr1, arr2);
            return arr1.front() + arr2.back() + arr1[N / 3];
        };
    }
    SECTION("Swap std::array") {
        std::array<int, N> arr1;
        std::array<int, N> arr2;
        for (size_t i = 0; i < N; ++i) {
            arr1[i] = i; // Ascending order
            arr2[i] = N - i; // Descending order
        }
        BENCHMARK("Swap std::array") {
            std::swap(arr1, arr2);
            return arr1.front() + arr2.back() + arr1[N / 3];
        };
    }
}