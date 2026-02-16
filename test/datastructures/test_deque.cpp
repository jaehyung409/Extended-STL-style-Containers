/*
 * @ Created by jaehyung409 on 25. 8. 22.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <deque>
#include <iterator>
#include <random>
import j;

constexpr size_t N = 1000;
std::random_device rd;
std::mt19937 gen(42);
std::uniform_int_distribution<> dis(0, static_cast<int>(N - 1));

TEST_CASE("Deque Basic") {
    SECTION("Constructors") {
        j::deque<int> default_d;
        REQUIRE(default_d.empty());

        j::deque<int> size_d(5);
        REQUIRE(size_d.size() == 5);
        for (auto &x : size_d)
            REQUIRE(x == 0);

        j::deque<int> val_d(3, 42);
        REQUIRE(val_d.size() == 3);
        for (auto &x : val_d)
            REQUIRE(x == 42);

        j::deque<int> src = {1, 2, 3};
        j::deque<int> iter_d(src.begin(), src.end());
        REQUIRE(iter_d.size() == src.size());

        j::deque<int> copy_d(iter_d);
        REQUIRE(copy_d == iter_d);

        j::deque<int> move_src = {7, 8, 9};
        j::deque<int> move_d(std::move(move_src));
        REQUIRE(move_d.size() == 3);
        REQUIRE(move_src.empty());

        j::deque<int> init_d = {4, 5, 6};
        REQUIRE(init_d.size() == 3);
        REQUIRE(init_d[0] == 4);
        REQUIRE(init_d[1] == 5);
        REQUIRE(init_d[2] == 6);
    }

    SECTION("Assignment") {
        j::deque<int> src = {1, 2, 3};
        j::deque<int> copy_assign;
        copy_assign = src;
        REQUIRE(copy_assign == src);

        j::deque<int> move_src = {10, 20, 30};
        j::deque<int> move_assign;
        move_assign = std::move(move_src);
        REQUIRE(move_assign.size() == 3);
        REQUIRE(move_src.empty());

        j::deque<int> d;
        d.assign(4, 99);
        REQUIRE(d.size() == 4);
        for (auto x : d)
            REQUIRE(x == 99);

        d.assign(src.begin(), src.end());
        REQUIRE(d.size() == src.size());

        d.assign({15, 25, 35});
        REQUIRE(d.size() == 3);
    }

    SECTION("Iterators") {
        j::deque<int> d = {1, 2, 3, 4, 5};
        using It = decltype(d.begin());
        REQUIRE(std::is_same_v<typename std::iterator_traits<It>::iterator_category, std::random_access_iterator_tag>);

        auto it = d.begin();
        REQUIRE(*it == 1);
        REQUIRE(*(it + 2) == 3);
        ++it;
        REQUIRE(*it == 2);
        --it;
        REQUIRE(*it == 1);
        it += 3;
        REQUIRE(*it == 4);
        it -= 2;
        REQUIRE(*it == 2);
        REQUIRE(it[1] == 3);

        auto rit = d.rbegin();
        REQUIRE(*rit == 5);
        ++rit;
        REQUIRE(*rit == 4);

        const auto &cd = d;
        REQUIRE(*cd.cbegin() == 1);
        REQUIRE(std::distance(d.begin(), d.end()) == 5);
        REQUIRE(std::distance(d.rbegin(), d.rend()) == 5);
    }

    SECTION("Capacity") {
        j::deque<int> d;
        REQUIRE(d.empty());
        REQUIRE(d.max_size() > 0);

        d.resize(3);
        REQUIRE(d.size() == 3);

        d.resize(5, 7);
        REQUIRE(d.size() == 5);
        REQUIRE(d.back() == 7);

        d.clear();
        REQUIRE(d.empty());
    }

    SECTION("Element Access") {
        j::deque<int> d = {1, 2, 3};
        REQUIRE(d[0] == 1);
        REQUIRE(d.at(1) == 2);
        REQUIRE(d.front() == 1);
        REQUIRE(d.back() == 3);
        REQUIRE_THROWS_AS(d.at(10), std::out_of_range);

        const auto &cd = d;
        REQUIRE(cd[0] == 1);
        REQUIRE(cd.at(1) == 2);
        REQUIRE(cd.front() == 1);
        REQUIRE(cd.back() == 3);
    }

    SECTION("Push and Pop") {
        j::deque<int> d;
        d.push_back(10);
        d.push_front(5);
        d.emplace_back(20);
        d.emplace_front(1);
        REQUIRE(d.size() == 4);
        REQUIRE(d.front() == 1);
        REQUIRE(d.back() == 20);

        d.pop_back();
        d.pop_front();
        REQUIRE(d.size() == 2);
        REQUIRE(d.front() == 5);
        REQUIRE(d.back() == 10);
    }

    SECTION("Insert and Erase") {
        j::deque<int> d = {1, 4};
        auto pos = d.emplace(d.begin() + 1, 2);
        REQUIRE(*pos == 2);

        d.insert(d.begin() + 2, 3);
        REQUIRE(d.size() == 4);
        REQUIRE(d[2] == 3);

        d.insert(d.begin(), 2, 0);
        REQUIRE(d[0] == 0);
        REQUIRE(d[1] == 0);

        j::deque<int> r = {6, 7};
        d.insert(d.end(), r.begin(), r.end());

        d.insert(d.begin(), {-2, -1});

        auto erase_it = d.erase(d.begin());
        REQUIRE(erase_it == d.begin());

        d.erase(d.begin(), d.begin() + 2);
        REQUIRE(d.size() > 0);
    }

    SECTION("Swap") {
        j::deque<int> a = {1, 2}, b = {3, 4};
        a.swap(b);
        REQUIRE(a.front() == 3);
        REQUIRE(b.front() == 1);

        j::swap(a, b);
        REQUIRE(a.front() == 1);
    }

    SECTION("Comparison") {
        j::deque<int> d1 = {1, 2, 3};
        j::deque<int> d2 = {1, 2, 3};
        j::deque<int> d3 = {3, 2, 1};
        j::deque<int> d4 = {1, 2, 3, 4};

        REQUIRE(d1 == d2);
        REQUIRE(d1 != d3);
        REQUIRE(d1 != d4);
        REQUIRE((d1 <=> d2) == std::strong_ordering::equal);
        REQUIRE((d1 <=> d3) == std::strong_ordering::less);
        REQUIRE((d3 <=> d1) == std::strong_ordering::greater);
    }
}

TEST_CASE("Deque Edge Cases") {
    SECTION("Empty Deque") {
        j::deque<int> d;
        REQUIRE(d.begin() == d.end());
        REQUIRE(d.cbegin() == d.cend());
        REQUIRE(d.rbegin() == d.rend());
        REQUIRE(d.empty());

        d.resize(3, 42);
        REQUIRE(d.size() == 3);
        for (auto &val : d)
            REQUIRE(val == 42);

        d.clear();
        REQUIRE(d.empty());
    }

    SECTION("Single Element") {
        j::deque<int> d = {42};
        REQUIRE(d.front() == 42);
        REQUIRE(d.back() == 42);
        REQUIRE(d.size() == 1);
        REQUIRE(d[0] == 42);
        REQUIRE(d.at(0) == 42);

        d.pop_back();
        REQUIRE(d.empty());
    }

    SECTION("Insert at Extremes") {
        j::deque<int> d;
        for (int i = 0; i < 100; ++i) {
            d.push_back(i);
        }

        // Insert at position 1
        d.insert(d.begin() + 1, 1111);
        REQUIRE(d.size() == 101);
        REQUIRE(d[0] == 0);
        REQUIRE(d[1] == 1111);
        REQUIRE(d[2] == 1);

        // Insert at end-1
        std::vector<int> to_insert(20, 888);
        auto pos = d.begin() + (d.size() - 1);
        d.insert(pos, to_insert.begin(), to_insert.end());
        REQUIRE(d.size() == 121);
    }

    SECTION("Iterator Stability After Modification") {
        j::deque<int> d = {1, 2, 3, 4, 5};
        auto it = d.begin() + 2;
        REQUIRE(*it == 3);

        d.insert(d.begin(), 0);
        REQUIRE(d[3] == 3);

        d.erase(d.begin());
        REQUIRE(d.size() == 5);
    }

    SECTION("Forward and Reverse Iteration") {
        j::deque<int> d;
        for (int i = 0; i < 100; ++i)
            d.push_back(i);

        // Forward iteration
        int expected = 0;
        for (auto x : d) {
            REQUIRE(x == expected++);
        }

        // Reverse iteration
        expected = 99;
        for (auto it = d.rbegin(); it != d.rend(); ++it) {
            REQUIRE(*it == expected--);
        }

        // Const iteration
        const j::deque<int> &cd = d;
        int sum = 0;
        for (auto it = cd.cbegin(); it != cd.cend(); ++it)
            sum += *it;
        REQUIRE(sum == 4950);
    }
}

TEST_CASE("Deque Large Dataset") {
    SECTION("Push Back Operations") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i)
            d.push_back(i);
        REQUIRE(d.size() == N);

        int v = 1;
        for (auto it = d.begin(); it != d.end(); ++it, ++v) {
            REQUIRE(*it == v);
        }
    }

    SECTION("Push Front Operations") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i)
            d.push_front(i);
        REQUIRE(d.size() == N);

        int v = N;
        for (auto it = d.begin(); it != d.end(); ++it, --v) {
            REQUIRE(*it == v);
        }
    }

    SECTION("Pop Back Operations") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i)
            d.push_back(i);

        for (int i = N; i >= 1; --i) {
            REQUIRE(d.back() == i);
            d.pop_back();
        }
        REQUIRE(d.empty());
    }

    SECTION("Pop Front Operations") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i)
            d.push_back(i);

        for (int i = 1; i <= N; ++i) {
            REQUIRE(d.front() == i);
            d.pop_front();
        }
        REQUIRE(d.empty());
    }

    SECTION("Erase Range") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i)
            d.push_back(i);

        d.erase(d.begin() + N / 4, d.begin() + 3 * N / 4);
        REQUIRE(d.size() == N / 2);

        int v = 1;
        for (auto it = d.begin(); it != d.end(); ++it, ++v) {
            if (v == N / 4 + 1)
                v = 3 * N / 4 + 1;
            REQUIRE(*it == v);
        }
    }

    SECTION("Erase Single Element") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i)
            d.push_back(i);

        size_t erase_pos = N / 2;
        d.erase(d.begin() + erase_pos);
        REQUIRE(d.size() == N - 1);

        for (size_t i = 0; i < erase_pos; ++i) {
            REQUIRE(d[i] == static_cast<int>(i + 1));
        }
        for (size_t i = erase_pos; i < d.size(); ++i) {
            REQUIRE(d[i] == static_cast<int>(i + 2));
        }
    }

    SECTION("Insert Middle Element") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i)
            d.push_back(i);

        auto mid_pos = d.begin() + N / 2;
        d.emplace(mid_pos, -999);
        REQUIRE(d.size() == N + 1);
        REQUIRE(d[N / 2] == -999);

        for (int i = 0; i < N / 2; ++i) {
            REQUIRE(d[i] == i + 1);
        }
        for (int i = N / 2 + 1; i < N + 1; ++i) {
            REQUIRE(d[i] == i);
        }
    }

    SECTION("Insert Range") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i)
            d.push_back(i);

        std::vector<int> range_to_insert;
        for (int i = 2000; i < 2000 + N; ++i) {
            range_to_insert.push_back(i);
        }

        auto insert_pos = d.begin() + N / 3;
        d.insert(insert_pos, range_to_insert.begin(), range_to_insert.end());
        REQUIRE(d.size() == N + N);

        for (int i = 0; i < N / 3; ++i) {
            REQUIRE(d[i] == i + 1);
        }
        for (int i = N / 3; i < N / 3 + N; ++i) {
            REQUIRE(d[i] == 2000 + (i - N / 3));
        }
        for (int i = N / 3 + N; i < 2 * N; ++i) {
            REQUIRE(d[i] == (i - N) + 1);
        }
    }

    SECTION("Insert Near Front - Count <= Distance") {
        j::deque<int> d;
        for (int i = 0; i < 10000; ++i)
            d.push_back(i);

        std::vector<int> to_insert;
        for (int i = 0; i < 50; ++i) {
            to_insert.push_back(i + 70000);
        }

        auto pos = d.begin() + 100;
        d.insert(pos, to_insert.begin(), to_insert.end());
        REQUIRE(d.size() == 10050);

        for (int i = 0; i < 50; ++i) {
            REQUIRE(d[100 + i] == 70000 + i);
        }
        for (int i = 0; i < 100; ++i) {
            REQUIRE(d[i] == i);
        }
    }

    SECTION("Insert Near End - Count <= Distance") {
        j::deque<int> d;
        for (int i = 0; i < 10000; ++i)
            d.push_back(i);

        std::vector<int> to_insert;
        for (int i = 0; i < 50; ++i) {
            to_insert.push_back(i + 50000);
        }

        auto pos = d.begin() + 9900;
        d.insert(pos, to_insert.begin(), to_insert.end());
        REQUIRE(d.size() == 10050);

        for (int i = 0; i < 50; ++i) {
            REQUIRE(d[9900 + i] == 50000 + i);
        }
        for (int i = 0; i < 100; ++i) {
            REQUIRE(d[9950 + i] == 9900 + i);
        }
    }

    SECTION("Insert Near End - Count > Distance") {
        j::deque<int> d;
        for (int i = 0; i < 10000; ++i)
            d.push_back(i);

        std::vector<int> to_insert;
        for (int i = 0; i < 500; ++i) {
            to_insert.push_back(i + 60000);
        }

        auto pos = d.begin() + 9950;
        d.insert(pos, to_insert.begin(), to_insert.end());
        REQUIRE(d.size() == 10500);

        for (int i = 0; i < 500; ++i) {
            REQUIRE(d[9950 + i] == 60000 + i);
        }
        for (int i = 0; i < 50; ++i) {
            REQUIRE(d[10450 + i] == 9950 + i);
        }
    }

    SECTION("Mixed Front and Back Operations") {
        j::deque<int> d;
        for (int i = 0; i < N / 2; ++i) {
            d.push_back(i);
            d.push_front(-i - 1);
        }
        REQUIRE(d.size() == N);
        REQUIRE(d.front() == -(N / 2));
        REQUIRE(d.back() == (N / 2 - 1));

        for (int i = 0; i < N / 4; ++i) {
            d.pop_front();
            d.pop_back();
        }
        REQUIRE(d.size() == N / 2);
    }

    SECTION("Resize Larger Then Smaller") {
        j::deque<int> d;
        for (int i = 0; i < 100; ++i)
            d.push_back(i);

        d.resize(N, 999);
        REQUIRE(d.size() == N);
        for (int i = 100; i < N; ++i) {
            REQUIRE(d[i] == 999);
        }

        d.resize(50);
        REQUIRE(d.size() == 50);
        for (int i = 0; i < 50; ++i) {
            REQUIRE(d[i] == i);
        }
    }

    SECTION("Insert Multiple Times at Different Positions") {
        j::deque<int> d;
        for (int i = 0; i < N; ++i)
            d.push_back(i);

        d.insert(d.begin() + N / 4, 100, -1);
        REQUIRE(d.size() == N + 100);

        d.insert(d.begin() + N / 2, 50, -2);
        REQUIRE(d.size() == N + 150);

        d.insert(d.end() - 10, 75, -3);
        REQUIRE(d.size() == N + 225);
    }

    SECTION("Erase and Insert Pattern") {
        j::deque<int> d;
        for (int i = 0; i < N; ++i)
            d.push_back(i);

        d.erase(d.begin() + 100, d.begin() + 200);
        REQUIRE(d.size() == N - 100);

        std::vector<int> to_insert(150, 777);
        d.insert(d.begin() + 100, to_insert.begin(), to_insert.end());
        REQUIRE(d.size() == N + 50);
    }

    SECTION("Continuous Push and Pop Pattern") {
        j::deque<int> d;
        for (int i = 0; i < N; ++i) {
            d.push_back(i);
            if (i % 3 == 0 && !d.empty()) {
                d.pop_front();
            }
        }
        REQUIRE(d.size() > 0);

        while (!d.empty()) {
            d.pop_back();
        }
        REQUIRE(d.empty());
    }
}

TEST_CASE("Deque Buffer Boundary") {
    SECTION("Operations Across Buffer Boundaries") {
        j::deque<int> d;
        // Push enough to span multiple buffers
        for (int i = 0; i < 2000; ++i) {
            d.push_back(i);
        }

        // Verify iterator works across buffers
        int count = 0;
        for (auto it = d.begin(); it != d.end(); ++it) {
            REQUIRE(*it == count++);
        }
        REQUIRE(count == 2000);

        // Insert at buffer boundary positions
        d.insert(d.begin() + 512, -1);
        d.insert(d.begin() + 1024, -2);
        REQUIRE(d.size() == 2002);
    }

    SECTION("Erase Across Buffer Boundaries") {
        j::deque<int> d;
        for (int i = 0; i < 3000; ++i) {
            d.push_back(i);
        }

        // Erase range that spans multiple buffers
        d.erase(d.begin() + 500, d.begin() + 1500);
        REQUIRE(d.size() == 2000);

        for (int i = 0; i < 500; ++i) {
            REQUIRE(d[i] == i);
        }
        for (int i = 500; i < 2000; ++i) {
            REQUIRE(d[i] == i + 1000);
        }
    }

    SECTION("Reverse Iterator Across Buffers") {
        j::deque<int> d;
        for (int i = 0; i < 1500; ++i) {
            d.push_back(i);
        }

        int expected = 1499;
        for (auto it = d.rbegin(); it != d.rend(); ++it) {
            REQUIRE(*it == expected--);
        }
        REQUIRE(expected == -1);
    }

    SECTION("Random Access Across Buffers") {
        j::deque<int> d;
        for (int i = 0; i < 2500; ++i) {
            d.push_back(i);
        }

        auto it = d.begin();
        it += 1000;
        REQUIRE(*it == 1000);

        it -= 500;
        REQUIRE(*it == 500);

        auto it2 = d.begin() + 2000;
        REQUIRE(*it2 == 2000);

        REQUIRE(it2 - it == 1500);
        REQUIRE(it[100] == 600);
    }

    SECTION("Map Reallocation - Front") {
        j::deque<int> d;
        // Force map reallocation by repeatedly pushing to front
        for (int i = 0; i < 5000; ++i) {
            d.push_front(i);
        }
        REQUIRE(d.size() == 5000);

        for (int i = 0; i < 5000; ++i) {
            REQUIRE(d[i] == 5000 - i - 1);
        }
    }

    SECTION("Map Reallocation - Back") {
        j::deque<int> d;
        // Force map reallocation by repeatedly pushing to back
        for (int i = 0; i < 5000; ++i) {
            d.push_back(i);
        }
        REQUIRE(d.size() == 5000);

        for (int i = 0; i < 5000; ++i) {
            REQUIRE(d[i] == i);
        }
    }

    SECTION("Alternating Front and Back with Map Reallocation") {
        j::deque<int> d;
        for (int i = 0; i < 1000; ++i) {
            d.push_front(i);
            d.push_back(-i);
        }
        REQUIRE(d.size() == 2000);
        REQUIRE(d.front() == 999);
        REQUIRE(d.back() == -999);
    }
}

TEST_CASE("Deque Special Operations") {
    SECTION("Shrink to Fit") {
        j::deque<int> d;
        for (int i = 0; i < 1000; ++i) {
            d.push_back(i);
        }

        d.resize(100);
        d.shrink_to_fit();
        REQUIRE(d.size() == 100);

        for (int i = 0; i < 100; ++i) {
            REQUIRE(d[i] == i);
        }
    }

    SECTION("Resize with Value - Large Increase") {
        j::deque<int> d = {1, 2, 3};
        d.resize(1000, 42);
        REQUIRE(d.size() == 1000);

        for (int i = 0; i < 3; ++i) {
            REQUIRE(d[i] == i + 1);
        }
        for (int i = 3; i < 1000; ++i) {
            REQUIRE(d[i] == 42);
        }
    }

    SECTION("Resize Down to Zero") {
        j::deque<int> d;
        for (int i = 0; i < 500; ++i) {
            d.push_back(i);
        }

        d.resize(0);
        REQUIRE(d.empty());
        REQUIRE(d.size() == 0);
    }

    SECTION("Erase Global Function") {
        j::deque<int> d = {1, 2, 3, 2, 4, 2, 5};
        auto removed = j::erase(d, 2);
        REQUIRE(removed == 3);
        REQUIRE(d.size() == 4);
        REQUIRE(d[0] == 1);
        REQUIRE(d[1] == 3);
        REQUIRE(d[2] == 4);
        REQUIRE(d[3] == 5);
    }

    SECTION("Erase If Global Function") {
        j::deque<int> d;
        for (int i = 0; i < 100; ++i) {
            d.push_back(i);
        }

        auto removed = j::erase_if(d, [](int x) { return x % 2 == 0; });
        REQUIRE(removed == 50);
        REQUIRE(d.size() == 50);

        for (int i = 0; i < 50; ++i) {
            REQUIRE(d[i] == 2 * i + 1);
        }
    }

    SECTION("Assign - Count and Value") {
        j::deque<int> d = {1, 2, 3};
        d.assign(100, 999);
        REQUIRE(d.size() == 100);

        for (auto x : d) {
            REQUIRE(x == 999);
        }
    }

    SECTION("Assign - Iterator Range") {
        std::vector<int> v;
        for (int i = 0; i < 200; ++i) {
            v.push_back(i * 2);
        }

        j::deque<int> d = {1, 2, 3};
        d.assign(v.begin(), v.end());
        REQUIRE(d.size() == 200);

        for (int i = 0; i < 200; ++i) {
            REQUIRE(d[i] == i * 2);
        }
    }

    SECTION("Assign - Initializer List") {
        j::deque<int> d;
        for (int i = 0; i < 100; ++i) {
            d.push_back(i);
        }

        d.assign({10, 20, 30, 40, 50});
        REQUIRE(d.size() == 5);
        REQUIRE(d[0] == 10);
        REQUIRE(d[4] == 50);
    }

    SECTION("Copy Constructor with Allocator") {
        j::deque<int> d1 = {1, 2, 3, 4, 5};
        j::deque<int> d2(d1, std::allocator<int>{});
        REQUIRE(d2 == d1);
        REQUIRE(d2.size() == 5);
    }

    SECTION("Move Constructor with Allocator") {
        j::deque<int> d1 = {1, 2, 3, 4, 5};
        j::deque<int> d2(std::move(d1), std::allocator<int>{});
        REQUIRE(d2.size() == 5);
        REQUIRE(d2[0] == 1);
        REQUIRE(d2[4] == 5);
    }

    SECTION("Initializer List Constructor with Allocator") {
        j::deque<int> d({10, 20, 30}, std::allocator<int>{});
        REQUIRE(d.size() == 3);
        REQUIRE(d[0] == 10);
        REQUIRE(d[2] == 30);
    }

    SECTION("Get Allocator") {
        j::deque<int> d;
        auto alloc = d.get_allocator();
        REQUIRE(std::is_same_v<decltype(alloc), std::allocator<int>>);
    }
}

TEST_CASE("Deque Iterator Advanced") {
    SECTION("Iterator Arithmetic - Complex") {
        j::deque<int> d;
        for (int i = 0; i < 1000; ++i) {
            d.push_back(i);
        }

        auto it1 = d.begin() + 100;
        auto it2 = d.begin() + 500;
        REQUIRE(it2 - it1 == 400);

        auto it3 = it2 - 300;
        REQUIRE(*it3 == 200);

        REQUIRE(it1 < it2);
        REQUIRE(it2 > it1);
        REQUIRE(it1 <= it2);
        REQUIRE(it2 >= it1);
        REQUIRE(it1 != it2);
    }

    SECTION("Const Iterator Operations") {
        j::deque<int> d = {1, 2, 3, 4, 5};
        const auto &cd = d;

        auto cit = cd.cbegin();
        REQUIRE(*cit == 1);
        ++cit;
        REQUIRE(*cit == 2);
        cit += 2;
        REQUIRE(*cit == 4);

        auto cit2 = cd.cend() - 1;
        REQUIRE(*cit2 == 5);
    }

    SECTION("Reverse Iterator Complex Operations") {
        j::deque<int> d;
        for (int i = 0; i < 500; ++i) {
            d.push_back(i);
        }

        auto rit = d.rbegin();
        REQUIRE(*rit == 499);
        rit += 100;
        REQUIRE(*rit == 399);

        auto rit2 = d.rend() - 50;
        REQUIRE(*rit2 == 49);

        REQUIRE(rit2 - rit > 0);
    }

    SECTION("Iterator Distance Calculation") {
        j::deque<int> d;
        for (int i = 0; i < 2000; ++i) {
            d.push_back(i);
        }

        REQUIRE(std::distance(d.begin(), d.end()) == 2000);
        REQUIRE(std::distance(d.cbegin(), d.cend()) == 2000);
        REQUIRE(std::distance(d.rbegin(), d.rend()) == 2000);
        REQUIRE(std::distance(d.crbegin(), d.crend()) == 2000);
    }

    SECTION("Iterator Subscript Operator") {
        j::deque<int> d;
        for (int i = 0; i < 300; ++i) {
            d.push_back(i);
        }

        auto it = d.begin() + 50;
        REQUIRE(it[0] == 50);
        REQUIRE(it[10] == 60);
        REQUIRE(it[100] == 150);
        REQUIRE(it[-10] == 40);
    }

    SECTION("Iterator Conversion") {
        j::deque<int> d = {1, 2, 3, 4, 5};
        auto it = d.begin();
        j::deque<int>::const_iterator cit = it;
        REQUIRE(*cit == 1);
    }
}

TEST_CASE("Deque Stress Tests") {
    SECTION("Large Insert at Begin") {
        j::deque<int> d;
        for (int i = 0; i < 1000; ++i) {
            d.push_back(i);
        }

        std::vector<int> to_insert(5000, -1);
        d.insert(d.begin(), to_insert.begin(), to_insert.end());
        REQUIRE(d.size() == 6000);

        for (int i = 0; i < 5000; ++i) {
            REQUIRE(d[i] == -1);
        }
        for (int i = 5000; i < 6000; ++i) {
            REQUIRE(d[i] == i - 5000);
        }
    }

    SECTION("Large Insert at End") {
        j::deque<int> d;
        for (int i = 0; i < 1000; ++i) {
            d.push_back(i);
        }

        std::vector<int> to_insert(5000, 999);
        d.insert(d.end(), to_insert.begin(), to_insert.end());
        REQUIRE(d.size() == 6000);

        for (int i = 1000; i < 6000; ++i) {
            REQUIRE(d[i] == 999);
        }
    }

    SECTION("Multiple Erase Operations") {
        j::deque<int> d;
        for (int i = 0; i < 5000; ++i) {
            d.push_back(i);
        }

        d.erase(d.begin(), d.begin() + 1000);
        REQUIRE(d.size() == 4000);

        d.erase(d.end() - 1000, d.end());
        REQUIRE(d.size() == 3000);

        d.erase(d.begin() + 500, d.begin() + 1500);
        REQUIRE(d.size() == 2000);
    }

    SECTION("Insert Rvalue") {
        j::deque<int> d = {1, 2, 3};
        d.insert(d.begin() + 1, 99);
        REQUIRE(d.size() == 4);
        REQUIRE(d[1] == 99);

        int value = 88;
        d.insert(d.begin() + 2, std::move(value));
        REQUIRE(d.size() == 5);
        REQUIRE(d[2] == 88);
    }

    SECTION("Push Rvalue") {
        j::deque<int> d;
        int x = 10, y = 20;
        d.push_back(std::move(x));
        d.push_front(std::move(y));
        REQUIRE(d.size() == 2);
        REQUIRE(d.front() == 20);
        REQUIRE(d.back() == 10);
    }

    SECTION("Emplace Operations") {
        j::deque<int> d = {1, 5};
        auto it = d.emplace(d.begin() + 1, 3);
        REQUIRE(*it == 3);
        REQUIRE(d.size() == 3);

        d.emplace_front(0);
        d.emplace_back(6);
        REQUIRE(d.size() == 5);
        REQUIRE(d.front() == 0);
        REQUIRE(d.back() == 6);
    }

    SECTION("Clear After Operations") {
        j::deque<int> d;
        for (int i = 0; i < 1000; ++i) {
            d.push_back(i);
            d.push_front(-i);
        }
        REQUIRE(d.size() == 2000);

        d.clear();
        REQUIRE(d.empty());
        REQUIRE(d.size() == 0);

        // Can still use after clear
        d.push_back(1);
        REQUIRE(d.size() == 1);
    }
}

TEST_CASE("Deque Regression: Middle Insert and Boundaries") {
    SECTION("Insert Middle Repeatedly (map reallocation boundary)") {
        j::deque<int> d;
        std::deque<int> expected;

        for (int i = 0; i < 100; ++i) {
            d.push_back(i);
            expected.push_back(i);
        }

        for (size_t i = 0; i < 1200; ++i) {
            INFO("iteration=" << i << ", size(before)=" << d.size());
            auto d_it = d.begin() + static_cast<std::ptrdiff_t>(d.size() / 2);
            auto e_it = expected.begin() + static_cast<std::ptrdiff_t>(expected.size() / 2);
            d.insert(d_it, 999);
            expected.insert(e_it, 999);

            REQUIRE(d.size() == expected.size());
            REQUIRE(d.front() == expected.front());
            REQUIRE(d.back() == expected.back());
            REQUIRE(d[d.size() / 2] == expected[expected.size() / 2]);
            REQUIRE(d[d.size() / 3] == expected[expected.size() / 3]);
            REQUIRE(d[(d.size() * 2) / 3] == expected[(expected.size() * 2) / 3]);
        }
    }

    SECTION("Emplace Middle Repeatedly (map reallocation boundary)") {
        j::deque<int> d;
        std::deque<int> expected;

        for (int i = 0; i < 100; ++i) {
            d.push_back(i);
            expected.push_back(i);
        }

        for (size_t i = 0; i < 1200; ++i) {
            INFO("iteration=" << i << ", size(before)=" << d.size());
            auto d_it = d.begin() + static_cast<std::ptrdiff_t>(d.size() / 2);
            auto e_it = expected.begin() + static_cast<std::ptrdiff_t>(expected.size() / 2);
            d.emplace(d_it, 777);
            expected.emplace(e_it, 777);

            REQUIRE(d.size() == expected.size());
            REQUIRE(d.front() == expected.front());
            REQUIRE(d.back() == expected.back());
            REQUIRE(d[d.size() / 2] == expected[expected.size() / 2]);
            REQUIRE(d[d.size() / 3] == expected[expected.size() / 3]);
            REQUIRE(d[(d.size() * 2) / 3] == expected[(expected.size() * 2) / 3]);
        }
    }

    SECTION("Zero-size constructor initializes internal map") {
        j::deque<int> d(0);
        REQUIRE(d.empty());
        REQUIRE(d.size() == 0);

        d.push_back(42);
        REQUIRE(d.size() == 1);
        REQUIRE(d.front() == 42);
        REQUIRE(d.back() == 42);
    }

    SECTION("Count insert at exact buffer multiple boundary") {
        constexpr size_t kIntBufferSize = 512 / sizeof(int);
        constexpr size_t kInsertCount = kIntBufferSize * 2;

        j::deque<int> d;
        std::deque<int> expected;

        for (int i = 0; i < 300; ++i) {
            d.push_back(i);
            expected.push_back(i);
        }

        auto d_pos = d.begin() + 150;
        auto e_pos = expected.begin() + 150;
        d.insert(d_pos, kInsertCount, -5);
        expected.insert(e_pos, kInsertCount, -5);

        REQUIRE(d.size() == expected.size());
        for (size_t i = 0; i < d.size(); ++i) {
            REQUIRE(d[i] == expected[i]);
        }
    }
}
