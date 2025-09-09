/*
 * @ Created by jaehyung409 on 25. 9. 9.
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <deque>
#include <list>

import j;

constexpr size_t N = 1000;
std::mt19937 gen(42);
std::uniform_int_distribution<> dis(0, static_cast<int>(N - 1));

TEST_CASE("Queue Basic") {
    SECTION("Constructors") {
        j::queue<int> q1;
        REQUIRE(q1.empty());
        REQUIRE(q1.size() == 0);

        std::vector<int> v = {1, 2, 3};
        j::queue<int> q2(v.begin(), v.end());
        REQUIRE(q2.size() == v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            REQUIRE(q2.front() == v[i]);
            q2.pop();
        }
        REQUIRE(q2.empty());

        j::queue<int> q3(q1);
        REQUIRE(q3 == q1);

        j::queue<int> q4(std::move(q3));
        REQUIRE(q4.empty());
        REQUIRE(q3.size() == 0);
    }

    SECTION("Assignment operators") {
        j::queue<int> q1;
        q1.push(1); q1.push(2);
        j::queue<int> q2;
        q2 = q1;
        REQUIRE(q2 == q1);
        j::queue<int> q3;
        q3 = std::move(q2);
        REQUIRE(q3.size() == 2);
        REQUIRE(q2.size() == 0);
    }

    SECTION("Push/Pop/Emplace/Front/Back") {
        j::queue<int> q;
        q.push(10);
        REQUIRE(q.front() == 10);
        REQUIRE(q.back() == 10);
        q.emplace(20);
        REQUIRE(q.front() == 10);
        REQUIRE(q.back() == 20);
        q.push(30);
        REQUIRE(q.front() == 10);
        REQUIRE(q.back() == 30);
        q.pop();
        REQUIRE(q.front() == 20);
        REQUIRE(q.back() == 30);
        q.pop();
        REQUIRE(q.front() == 30);
        REQUIRE(q.back() == 30);
        q.pop();
        REQUIRE(q.empty());
    }

    SECTION("Swap") {
        j::queue<int> a, b;
        a.push(1); a.push(2);
        b.push(3);
        a.swap(b);
        REQUIRE(a.size() == 1);
        REQUIRE(b.size() == 2);
        REQUIRE(a.front() == 3);
        REQUIRE(b.front() == 1);
        j::swap(a, b);
        REQUIRE(a.front() == 1);
        REQUIRE(b.front() == 3);
    }

    SECTION("Comparison operators") {
        j::queue<int> q1, q2;
        q1.push(1); q1.push(2);
        q2.push(1); q2.push(2);
        REQUIRE(q1 == q2);
        q2.pop(); q2.push(3);
        REQUIRE(q1 != q2);
    }
}

TEST_CASE("Queue Edge Cases") {
    SECTION("Single element operations") {
        j::queue<int> q;
        q.push(42);
        REQUIRE(q.front() == 42);
        REQUIRE(q.back() == 42);
        q.pop();
        REQUIRE(q.empty());
    }
}

TEST_CASE("Queue Large Dataset") {
    j::queue<int> q;
    for (size_t i = 0; i < N; ++i) {
        q.push(static_cast<int>(i));
    }
    REQUIRE(q.size() == N);
    for (size_t i = 0; i < N; ++i) {
        REQUIRE(q.front() == static_cast<int>(i));
        q.pop();
    }
    REQUIRE(q.empty());
}

TEST_CASE("Queue Container Variants") {
    SECTION("j::queue with j::deque") {
        j::queue<int, j::deque<int>> q;
        for (int i = 0; i < 10; ++i) q.push(i);
        REQUIRE(q.size() == 10);
        for (int i = 0; i < 10; ++i) {
            REQUIRE(q.front() == i);
            q.pop();
        }
        REQUIRE(q.empty());
    }
    SECTION("j::queue with std::deque") {
        j::queue<int, std::deque<int>> q;
        for (int i = 0; i < 10; ++i) q.push(i);
        REQUIRE(q.size() == 10);
        for (int i = 0; i < 10; ++i) {
            REQUIRE(q.front() == i);
            q.pop();
        }
        REQUIRE(q.empty());
    }
    SECTION("std::queue with j::deque") {
        std::queue<int, j::deque<int>> q;
        for (int i = 0; i < 10; ++i) q.push(i);
        REQUIRE(q.size() == 10);
        for (int i = 0; i < 10; ++i) {
            REQUIRE(q.front() == i);
            q.pop();
        }
        REQUIRE(q.empty());
    }
}

TEST_CASE("Priority Queue Basic") {
    SECTION("Default max heap behavior") {
        j::priority_queue<int> pq;
        pq.push(3);
        pq.push(1);
        pq.push(4);
        pq.push(2);

        REQUIRE(pq.top() == 4);
        pq.pop();
        REQUIRE(pq.top() == 3);
        pq.pop();
        REQUIRE(pq.top() == 2);
        pq.pop();
        REQUIRE(pq.top() == 1);
        pq.pop();
        REQUIRE(pq.empty());
    }

    SECTION("Min heap with greater comparator") {
        j::priority_queue<int, std::vector<int>, std::greater<int>> pq;
        pq.push(3);
        pq.push(1);
        pq.push(4);
        pq.push(2);

        REQUIRE(pq.top() == 1);
        pq.pop();
        REQUIRE(pq.top() == 2);
        pq.pop();
        REQUIRE(pq.top() == 3);
        pq.pop();
        REQUIRE(pq.top() == 4);
        pq.pop();
        REQUIRE(pq.empty());
    }

    SECTION("Constructor with range") {
        std::vector<int> v = {3, 1, 4, 2, 5};
        j::priority_queue<int> pq(v.begin(), v.end());

        REQUIRE(pq.top() == 5);
        pq.pop();
        REQUIRE(pq.top() == 4);
        pq.pop();
        REQUIRE(pq.top() == 3);
        pq.pop();
        REQUIRE(pq.top() == 2);
        pq.pop();
        REQUIRE(pq.top() == 1);
        pq.pop();
        REQUIRE(pq.empty());
    }

    SECTION("Constructor with container") {
        std::vector<int> v = {3, 1, 4, 2};
        j::priority_queue pq(std::less<int>(), v);

        REQUIRE(pq.size() == 4);
        REQUIRE(pq.top() == 4);
    }

    SECTION("Emplace operation") {
        j::priority_queue<int> pq;
        pq.emplace(3);
        pq.emplace(1);
        pq.emplace(4);

        REQUIRE(pq.top() == 4);
        pq.pop();
        REQUIRE(pq.top() == 3);
        pq.pop();
        REQUIRE(pq.top() == 1);
    }
}

TEST_CASE("Priority Queue Container Variants") {
    SECTION("j::priority_queue with j::vector") {
        j::priority_queue<int, j::vector<int>> pq;
        for (int i = 0; i < 10; ++i) pq.push(i);
        REQUIRE(pq.size() == 10);
        for (int i = 9; i >= 0; --i) {
            REQUIRE(pq.top() == i);
            pq.pop();
        }
        REQUIRE(pq.empty());
    }

    SECTION("j::priority_queue with std::vector") {
        j::priority_queue<int, std::vector<int>> pq;
        for (int i = 0; i < 10; ++i) pq.push(i);
        REQUIRE(pq.size() == 10);
        for (int i = 9; i >= 0; --i) {
            REQUIRE(pq.top() == i);
            pq.pop();
        }
        REQUIRE(pq.empty());
    }

    SECTION("std::priority_queue with j::vector") {
        std::priority_queue<int, j::vector<int>> pq;
        for (int i = 0; i < 10; ++i) pq.push(i);
        REQUIRE(pq.size() == 10);
        for (int i = 9; i >= 0; --i) {
            REQUIRE(pq.top() == i);
            pq.pop();
        }
        REQUIRE(pq.empty());
    }
}

TEST_CASE("Priority Queue Edge Cases") {
    SECTION("Single element") {
        j::priority_queue<int> pq;
        pq.push(42);
        REQUIRE(pq.top() == 42);
        pq.pop();
        REQUIRE(pq.empty());
    }

    SECTION("Duplicate elements") {
        j::priority_queue<int> pq;
        pq.push(5);
        pq.push(5);
        pq.push(5);

        REQUIRE(pq.size() == 3);
        while (!pq.empty()) {
            REQUIRE(pq.top() == 5);
            pq.pop();
        }
    }
}

TEST_CASE("Priority Queue Large Dataset") {
    SECTION("Random data max heap") {
        j::priority_queue<int> pq;
        std::vector<int> data;

        for (size_t i = 0; i < 100; ++i) {
            int val = dis(gen);
            data.push_back(val);
            pq.push(val);
        }

        std::sort(data.rbegin(), data.rend()); // Sort in descending order

        for (size_t i = 0; i < data.size(); ++i) {
            REQUIRE(pq.top() == data[i]);
            pq.pop();
        }
        REQUIRE(pq.empty());
    }

    SECTION("Random data min heap") {
        j::priority_queue<int, std::vector<int>, std::greater<int>> pq;
        std::vector<int> data;

        for (size_t i = 0; i < 100; ++i) {
            int val = dis(gen);
            data.push_back(val);
            pq.push(val);
        }

        std::sort(data.begin(), data.end()); // Sort in ascending order

        for (size_t i = 0; i < data.size(); ++i) {
            REQUIRE(pq.top() == data[i]);
            pq.pop();
        }
        REQUIRE(pq.empty());
    }
}
