/*
 * @ Created by jaehyung409 on 25. 8. 27..
 * @ Copyright (c) 2025 jaehyung409 
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>
#include <deque>
#include <list>

import j;

constexpr size_t N = 1000;
std::mt19937 gen(42);
std::uniform_int_distribution<> dis(0, static_cast<int>(N - 1));

TEST_CASE("Stack Basic") {
    SECTION("Constructors") {
        j::stack<int> s1;
        REQUIRE(s1.empty());
        REQUIRE(s1.size() == 0);

        std::vector<int> v = {1, 2, 3};
        j::stack<int> s2(v.begin(), v.end());
        REQUIRE(s2.size() == v.size());
        for (int i = (int)v.size() - 1; i >= 0; --i) {
            REQUIRE(s2.top() == v[i]);
            s2.pop();
        }
        REQUIRE(s2.empty());

        j::stack<int> s3(s1);
        REQUIRE(s3 == s1);

        j::stack<int> s4(std::move(s3));
        REQUIRE(s4.empty());
        REQUIRE(s3.size() == 0);
    }

    SECTION("Assignment operators") {
        j::stack<int> s1;
        s1.push(1); s1.push(2);
        j::stack<int> s2;
        s2 = s1;
        REQUIRE(s2 == s1);
        j::stack<int> s3;
        s3 = std::move(s2);
        REQUIRE(s3.size() == 2);
        REQUIRE(s2.size() == 0);
    }

    SECTION("Push/Pop/Emplace/Top") {
        j::stack<int> s;
        s.push(10);
        REQUIRE(s.top() == 10);
        s.emplace(20);
        REQUIRE(s.top() == 20);
        s.push(30);
        REQUIRE(s.top() == 30);
        s.pop();
        REQUIRE(s.top() == 20);
        s.pop();
        REQUIRE(s.top() == 10);
        s.pop();
        REQUIRE(s.empty());
    }

    SECTION("Swap") {
        j::stack<int> a, b;
        a.push(1); a.push(2);
        b.push(3);
        a.swap(b);
        REQUIRE(a.size() == 1);
        REQUIRE(b.size() == 2);
        REQUIRE(a.top() == 3);
        REQUIRE(b.top() == 2);
        j::swap(a, b);
        REQUIRE(a.top() == 2);
        REQUIRE(b.top() == 3);
    }

    SECTION("Comparison operators") {
        j::stack<int> s1, s2;
        s1.push(1); s1.push(2);
        s2.push(1); s2.push(2);
        REQUIRE(s1 == s2);
        s2.pop(); s2.push(3);
        REQUIRE(s1 != s2);
    }
}

TEST_CASE("Stack Edge Cases") {
    SECTION("Single element operations") {
        j::stack<int> s;
        s.push(42);
        REQUIRE(s.top() == 42);
        s.pop();
        REQUIRE(s.empty());
    }
}

TEST_CASE("Stack Large Dataset") {
    j::stack<int> s;
    for (size_t i = 0; i < N; ++i) {
        s.push(static_cast<int>(i));
    }
    REQUIRE(s.size() == N);
    for (size_t i = 0; i < N; ++i) {
        REQUIRE(s.top() == static_cast<int>(N - 1 - i));
        s.pop();
    }
    REQUIRE(s.empty());
}

TEST_CASE("Stack Container Variants") {
    SECTION("j::stack with j::vector") {
        j::stack<int, j::vector<int>> s;
        for (int i = 0; i < 10; ++i) s.push(i);
        REQUIRE(s.size() == 10);
        for (int i = 9; i >= 0; --i) {
            REQUIRE(s.top() == i);
            s.pop();
        }
        REQUIRE(s.empty());
    }
    SECTION("j::stack with j::deque") {
        j::stack<int, j::deque<int>> s;
        for (int i = 0; i < 10; ++i) s.push(i);
        REQUIRE(s.size() == 10);
        for (int i = 9; i >= 0; --i) {
            REQUIRE(s.top() == i);
            s.pop();
        }
        REQUIRE(s.empty());
    }
    SECTION("j::stack with std::vector") {
        j::stack<int, std::vector<int>> s;
        for (int i = 0; i < 10; ++i) s.push(i);
        REQUIRE(s.size() == 10);
        for (int i = 9; i >= 0; --i) {
            REQUIRE(s.top() == i);
            s.pop();
        }
        REQUIRE(s.empty());
    }
    SECTION("j::stack with std::deque") {
        j::stack<int, std::deque<int>> s;
        for (int i = 0; i < 10; ++i) s.push(i);
        REQUIRE(s.size() == 10);
        for (int i = 9; i >= 0; --i) {
            REQUIRE(s.top() == i);
            s.pop();
        }
        REQUIRE(s.empty());
    }
    SECTION("std::stack with j::vector") {
        std::stack<int, j::vector<int>> s;
        for (int i = 0; i < 10; ++i) s.push(i);
        REQUIRE(s.size() == 10);
        for (int i = 9; i >= 0; --i) {
            REQUIRE(s.top() == i);
            s.pop();
        }
        REQUIRE(s.empty());
    }
    SECTION("std::stack with j::deque") {
        std::stack<int, j::deque<int>> s;
        for (int i = 0; i < 10; ++i) s.push(i);
        REQUIRE(s.size() == 10);
        for (int i = 9; i >= 0; --i) {
            REQUIRE(s.top() == i);
            s.pop();
        }
        REQUIRE(s.empty());
    }
}
