/*
 * @ Created by jaehyung409 on 25. 8. 2.
 * @ Copyright (c) 2025 jaehyung409 
 * This software is licensed under the MIT License. 
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <array>
#include <algorithm>
import j.array;

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