/*
* @ Created by jaehyung409 on 25. 8. 3.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <algorithm>
#include <iterator>
#include <vector>
#include <random>
import j.vector;

constexpr size_t N = 1000;
std::random_device rd;
std::mt19937 gen(42); // Fixed seed for reproducibility
std::uniform_int_distribution<> dis(0, static_cast<int>(N - 1));

TEST_CASE("Vector Basic") {
    SECTION("Constructors") {
        // Default constructor
        j::vector<int> default_v;
        REQUIRE(default_v.empty());
        REQUIRE(default_v.size() == 0);

        // Size constructor
        j::vector<int> size_v(5);
        REQUIRE(size_v.size() == 5);
        for (auto& x : size_v) REQUIRE(x == 0);

        // Size + value constructor
        j::vector<int> val_v(3, 42);
        REQUIRE(val_v.size() == 3);
        for (auto& x : val_v) REQUIRE(x == 42);

        // Iterator constructor
        std::vector<int> src = {1, 2, 3};
        j::vector<int> iter_v(src.begin(), src.end());
        REQUIRE(iter_v.size() == src.size());
        auto it = iter_v.begin();
        for (auto v : src) REQUIRE(*it++ == v);

        // Copy constructor
        j::vector<int> copy_v(iter_v);
        REQUIRE(copy_v == iter_v);

        // Move constructor
        j::vector<int> move_src = {7, 8, 9};
        size_t old = move_src.size();
        j::vector<int> move_v(std::move(move_src));
        REQUIRE(move_v.size() == old);
        REQUIRE(move_src.size() == 0);
        it = move_v.begin(); REQUIRE(*it++ == 7);

        // Initializer list constructor
        j::vector<int> init_v = {4, 5, 6};
        REQUIRE(init_v.size() == 3);
        it = init_v.begin(); REQUIRE(*it++ == 4); REQUIRE(*it++ == 5); REQUIRE(*it++ == 6);

        // Constructor with allocator
        j::vector<int> alloc_v(std::allocator<int>{});
        REQUIRE(alloc_v.empty());
    }

    SECTION("Assignment operators") {
        j::vector<int> src = {1, 2, 3};

        // Copy assignment
        j::vector<int> copy_assign;
        copy_assign = src;
        REQUIRE(copy_assign == src);

        // Move assignment
        j::vector<int> move_src = {10, 20, 30};
        j::vector<int> move_assign;
        move_assign = std::move(move_src);
        REQUIRE(move_assign.size() == 3);
        REQUIRE(move_src.size() == 0);

        // Initializer list assignment
        j::vector<int> il_assign;
        il_assign = {11, 22, 33};
        REQUIRE(il_assign.size() == 3);
        auto it = il_assign.begin(); REQUIRE(*it++ == 11); REQUIRE(*it++ == 22); REQUIRE(*it++ == 33);
    }

    SECTION("Assign methods") {
        j::vector<int> v;

        // assign with count and value
        v.assign(4, 99);
        REQUIRE(v.size() == 4);
        for (auto x : v) REQUIRE(x == 99);

        // assign with iterators
        std::vector<int> tmp = {5, 6, 7};
        v.assign(tmp.begin(), tmp.end());
        REQUIRE(v.size() == tmp.size());
        auto it = v.begin(); for (auto x : tmp) REQUIRE(*it++ == x);

        // assign with initializer list
        v.assign({15, 25, 35});
        REQUIRE(v.size() == 3);
        it = v.begin(); REQUIRE(*it++ == 15); REQUIRE(*it++ == 25); REQUIRE(*it++ == 35);
    }

    SECTION("Iterators") {
        j::vector<int> v = {1, 2, 3, 4, 5};

        // Forward iterators
        auto it = v.begin(); REQUIRE(*it == 1);
        REQUIRE(*(it + 2) == 3);
        ++it; REQUIRE(*it == 2);
        --it; REQUIRE(*it == 1);
        it += 3; REQUIRE(*it == 4);
        it -= 2; REQUIRE(*it == 2);
        REQUIRE(it[1] == 3);

        // Reverse iterators
        auto rit = v.rbegin(); REQUIRE(*rit == 5);
        ++rit; REQUIRE(*rit == 4);
        --rit; REQUIRE(*rit == 5);

        // Const iterators
        const auto& cv = v;
        auto cit = cv.begin(); REQUIRE(*cit == 1);
        auto cit2 = v.cbegin(); REQUIRE(*cit2 == 1);

        REQUIRE(std::distance(v.begin(), v.end()) == 5);
        REQUIRE(std::distance(v.cbegin(), v.cend()) == 5);
        REQUIRE(std::distance(v.rbegin(), v.rend()) == 5);
        REQUIRE(std::distance(v.crbegin(), v.crend()) == 5);
    }

    SECTION("Capacity") {
        j::vector<int> v;
        REQUIRE(v.empty());
        REQUIRE(v.max_size() > 0);

        v.reserve(10);
        REQUIRE(v.capacity() >= 10);

        v.resize(3);
        REQUIRE(v.size() == 3);

        v.resize(5, 7);
        REQUIRE(v.size() == 5);
        REQUIRE(v.back() == 7);

        v.shrink_to_fit();
        REQUIRE(v.capacity() == v.size());

        v.clear();
        REQUIRE(v.empty());
    }

    SECTION("Element Access") {
        j::vector<int> v = {1, 2, 3};

        REQUIRE(v[0] == 1);
        REQUIRE(v.at(1) == 2);
        REQUIRE(v.front() == 1);
        REQUIRE(v.back() == 3);

        auto p = v.data();
        REQUIRE(p[2] == 3);

        // Test bounds checking
        REQUIRE_THROWS_AS(v.at(10), std::out_of_range);

        // Test const versions
        const auto& cv = v;
        REQUIRE(cv[0] == 1);
        REQUIRE(cv.at(1) == 2);
        REQUIRE(cv.front() == 1);
        REQUIRE(cv.back() == 3);
        REQUIRE(cv.data()[2] == 3);
    }

    SECTION("Modifiers") {
        j::vector<int> v;

        // push_back and emplace_back
        v.push_back(10);
        v.emplace_back(20);
        REQUIRE(v.size() == 2);
        REQUIRE(v.front() == 10);
        REQUIRE(v.back() == 20);

        // pop_back
        v.pop_back();
        REQUIRE(v.size() == 1);

        v.clear();
        v = {1, 4};

        // emplace
        auto pos = v.emplace(v.begin() + 1, 2);
        REQUIRE(*pos == 2);
        REQUIRE(v[1] == 2);

        // insert single element
        v.insert(v.begin() + 2, 3);
        REQUIRE(v[2] == 3);

        // insert multiple elements
        v.insert(v.begin(), 2, 5);
        REQUIRE(v.size() >= 5);
        REQUIRE(v[0] == 5);
        REQUIRE(v[1] == 5);

        // insert with iterators
        std::vector<int> r = {6, 7};
        v.insert(v.end(), r.begin(), r.end());
        REQUIRE(v.size() >= 7);

        // insert with initializer list
        v.insert(v.begin(), {-1, -2});
        REQUIRE(v.size() >= 9);

        // erase single element
        auto old_size = v.size();
        auto erase_it = v.erase(v.begin());
        REQUIRE(v.size() == old_size - 1);

        // erase range
        old_size = v.size();
        v.erase(v.begin() + 1, v.begin() + 3);
        REQUIRE(v.size() == old_size - 2);

        // swap
        j::vector<int> a = {1, 2}, b = {3, 4};
        a.swap(b);
        REQUIRE(a.front() == 3);
        REQUIRE(b.front() == 1);
        j::swap(a, b);
        REQUIRE(a.front() == 1);
    }

    SECTION("Comparison operators") {
        j::vector<int> v1 = {1, 2, 3};
        j::vector<int> v2 = {1, 2, 3};
        j::vector<int> v3 = {3, 2, 1};
        j::vector<int> v4 = {1, 2, 3, 4};

        REQUIRE(v1 == v2);
        REQUIRE(!(v1 != v2));
        REQUIRE(v1 != v3);
        REQUIRE(v1 != v4);

        REQUIRE((v1 <=> v2) == std::strong_ordering::equal);
        REQUIRE((v1 <=> v3) == std::strong_ordering::less);
        REQUIRE((v3 <=> v1) == std::strong_ordering::greater);
        REQUIRE((v1 <=> v4) == std::strong_ordering::less);
    }

    SECTION("Allocator") {
        j::vector<int> v;
        auto alloc = v.get_allocator();
        REQUIRE(std::is_same_v<decltype(alloc), std::allocator<int>>);
    }
}

TEST_CASE("Vector Edge Cases") {
    SECTION("Empty vector operations") {
        j::vector<int> empty_v;

        // Test iterators on empty vector
        REQUIRE(empty_v.begin() == empty_v.end());
        REQUIRE(empty_v.cbegin() == empty_v.cend());
        REQUIRE(empty_v.rbegin() == empty_v.rend());

        // Clear empty vector
        empty_v.clear();
        REQUIRE(empty_v.empty());

        // Resize empty vector
        empty_v.resize(3, 42);
        REQUIRE(empty_v.size() == 3);
        for (auto& val : empty_v) {
            REQUIRE(val == 42);
        }
    }

    SECTION("Single element operations") {
        j::vector<int> single_v = {42};

        REQUIRE(single_v.front() == 42);
        REQUIRE(single_v.back() == 42);
        REQUIRE(single_v.size() == 1);
        REQUIRE(single_v[0] == 42);
        REQUIRE(single_v.at(0) == 42);

        single_v.pop_back();
        REQUIRE(single_v.empty());
    }

    SECTION("Large capacity operations") {
        j::vector<int> v;
        v.reserve(1000);
        REQUIRE(v.capacity() >= 1000);
        REQUIRE(v.size() == 0);

        v.shrink_to_fit();
        REQUIRE(v.capacity() == 0);
    }

    SECTION("Iterator invalidation scenarios") {
        j::vector<int> v = {1, 2, 3, 4, 5};
        auto it = v.begin() + 2;
        REQUIRE(*it == 3);

        // Insert should maintain relative positions
        v.insert(v.begin(), 0);
        // Note: iterator is invalidated after insert
        REQUIRE(v[3] == 3); // Element moved to index 3
    }
}

TEST_CASE("Vector Large Dataset") {
    SECTION("Push and Pop operations") {
        j::vector<int> v;

        // Push back many elements
        for (size_t i = 0; i < N; ++i) {
            v.push_back(static_cast<int>(i));
        }
        REQUIRE(v.size() == N);

        // Verify elements
        for (size_t i = 0; i < N; ++i) {
            REQUIRE(v[i] == static_cast<int>(i));
        }

        // Pop back all elements
        for (size_t i = 0; i < N; ++i) {
            REQUIRE(v.back() == static_cast<int>(N - 1 - i));
            v.pop_back();
        }
        REQUIRE(v.empty());
    }

    SECTION("Insert and Erase operations") {
        j::vector<int> v;
        for (size_t i = 0; i < N; ++i) {
            v.push_back(static_cast<int>(i));
        }
        REQUIRE(v.size() == N);

        // Insert in middle
        for (size_t i = 0; i < 500; ++i) {
            v.insert(v.begin() + v.size()/2, 999);
        }
        REQUIRE(v.size() == N + 500);

        // Erase from beginning
        for (size_t i = 0; i < 500; ++i) {
            v.erase(v.begin());
        }
        REQUIRE(v.size() == N);
    }

    SECTION("Resize operations") {
        j::vector<int> v;

        // Resize up with default value
        v.resize(N);
        REQUIRE(v.size() == N);
        for (const auto& val : v) {
            REQUIRE(val == 0);
        }

        // Resize up with specific value
        v.resize(N + 500, 42);
        REQUIRE(v.size() == N + 500);
        for (size_t i = N; i < N + 500; ++i) {
            REQUIRE(v[i] == 42);
        }

        // Resize down
        v.resize(N / 2);
        REQUIRE(v.size() == N / 2);
    }

    SECTION("Iterator performance") {
        j::vector<int> v;
        for (size_t i = 0; i < N; ++i) {
            v.push_back(static_cast<int>(i));
        }

        // Forward iteration
        size_t sum = 0;
        for (auto it = v.begin(); it != v.end(); ++it) {
            sum += *it;
        }
        REQUIRE(sum == (N * (N - 1)) / 2);

        // Reverse iteration
        sum = 0;
        for (auto it = v.rbegin(); it != v.rend(); ++it) {
            sum += *it;
        }
        REQUIRE(sum == (N * (N - 1)) / 2);

        // Range-based for loop
        sum = 0;
        for (const auto& val : v) {
            sum += val;
        }
        REQUIRE(sum == (N * (N - 1)) / 2);
    }

    SECTION("Memory management") {
        j::vector<int> v;
        v.reserve(N);
        auto initial_capacity = v.capacity();

        for (size_t i = 0; i < N / 2; ++i) {
            v.push_back(static_cast<int>(i));
        }
        REQUIRE(v.capacity() == initial_capacity); // Should not reallocate

        v.shrink_to_fit();
        REQUIRE(v.capacity() == v.size());
    }

    SECTION("Copy and move semantics") {
        j::vector<int> original;
        for (size_t i = 0; i < N; ++i) {
            original.push_back(static_cast<int>(i));
        }

        // Copy constructor
        j::vector<int> copied(original);
        REQUIRE(copied.size() == original.size());
        REQUIRE(copied == original);

        // Move constructor
        j::vector<int> moved(std::move(original));
        REQUIRE(moved.size() == N);
        REQUIRE(original.size() == 0);

        // Copy assignment
        j::vector<int> copy_assigned;
        copy_assigned = copied;
        REQUIRE(copy_assigned == copied);

        // Move assignment
        j::vector<int> move_assigned;
        move_assigned = std::move(copied);
        REQUIRE(move_assigned.size() == N);
        REQUIRE(copied.size() == 0);
    }

    SECTION("Random access performance") {
        j::vector<int> v;

        for (size_t i = 0; i < N; ++i) {
            v.push_back(static_cast<int>(i));
        }

        // Random access test
        for (size_t i = 0; i < 100; ++i) {
            auto index = dis(gen);
            REQUIRE(v[index] == index);
            REQUIRE(v.at(index) == index);
        }
    }
}
