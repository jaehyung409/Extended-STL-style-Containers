/*
 * @ Created by jaehyung409 on 25. 9. 9.
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <deque>
#include <random>
#include <queue>
#include <algorithm>

import j;

constexpr size_t N = 1000;
std::random_device rd;
std::mt19937 gen(42); // Fixed seed for reproducibility
std::uniform_int_distribution<> dis(1, 10000);

TEST_CASE("Heap Algorithm Benchmarks: make_heap") {
    SECTION("j::vector containers") {
        BENCHMARK("j::make_heap with j::vector") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            j::make_heap(v.begin(), v.end());
            return v.size();
        };

        BENCHMARK("std::make_heap with j::vector") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::make_heap(v.begin(), v.end());
            return v.size();
        };
    }

    SECTION("std::vector containers") {
        BENCHMARK("j::make_heap with std::vector") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            j::make_heap(v.begin(), v.end());
            return v.size();
        };

        BENCHMARK("std::make_heap with std::vector") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::make_heap(v.begin(), v.end());
            return v.size();
        };
    }

    SECTION("j::deque containers") {
        BENCHMARK("j::make_heap with j::deque") {
            j::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            j::make_heap(v.begin(), v.end());
            return v.size();
        };

        BENCHMARK("std::make_heap with j::deque") {
            j::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::make_heap(v.begin(), v.end());
            return v.size();
        };
    }

    SECTION("std::deque containers") {
        BENCHMARK("j::make_heap with std::deque") {
            std::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            j::make_heap(v.begin(), v.end());
            return v.size();
        };

        BENCHMARK("std::make_heap with std::deque") {
            std::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::make_heap(v.begin(), v.end());
            return v.size();
        };
    }
}

TEST_CASE("Heap Algorithm Benchmarks: push_heap") {
    SECTION("j::vector containers") {
        BENCHMARK("j::push_heap with j::vector") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
                j::push_heap(v.begin(), v.end());
            }
            return v.size();
        };

        BENCHMARK("std::push_heap with j::vector") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
                std::push_heap(v.begin(), v.end());
            }
            return v.size();
        };
    }

    SECTION("std::vector containers") {
        BENCHMARK("j::push_heap with std::vector") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
                j::push_heap(v.begin(), v.end());
            }
            return v.size();
        };

        BENCHMARK("std::push_heap with std::vector") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
                std::push_heap(v.begin(), v.end());
            }
            return v.size();
        };
    }

    SECTION("j::deque containers") {
        BENCHMARK("j::push_heap with j::deque") {
            j::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
                j::push_heap(v.begin(), v.end());
            }
            return v.size();
        };

        BENCHMARK("std::push_heap with j::deque") {
            j::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
                std::push_heap(v.begin(), v.end());
            }
            return v.size();
        };
    }

    SECTION("std::deque containers") {
        BENCHMARK("j::push_heap with std::deque") {
            std::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
                j::push_heap(v.begin(), v.end());
            }
            return v.size();
        };

        BENCHMARK("std::push_heap with std::deque") {
            std::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
                std::push_heap(v.begin(), v.end());
            }
            return v.size();
        };
    }
}

TEST_CASE("Heap Algorithm Benchmarks: pop_heap") {
    SECTION("j::vector containers") {
        BENCHMARK("j::pop_heap with j::vector") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            j::make_heap(v.begin(), v.end());
            for (size_t i = 0; i < N; ++i) {
                j::pop_heap(v.begin(), v.end() - i);
            }
            return v.size();
        };

        BENCHMARK("std::pop_heap with j::vector") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::make_heap(v.begin(), v.end());
            for (size_t i = 0; i < N; ++i) {
                std::pop_heap(v.begin(), v.end() - i);
            }
            return v.size();
        };
    }

    SECTION("std::vector containers") {
        BENCHMARK("j::pop_heap with std::vector") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            j::make_heap(v.begin(), v.end());
            for (size_t i = 0; i < N; ++i) {
                j::pop_heap(v.begin(), v.end() - i);
            }
            return v.size();
        };

        BENCHMARK("std::pop_heap with std::vector") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::make_heap(v.begin(), v.end());
            for (size_t i = 0; i < N; ++i) {
                std::pop_heap(v.begin(), v.end() - i);
            }
            return v.size();
        };
    }

    SECTION("j::deque containers") {
        BENCHMARK("j::pop_heap with j::deque") {
            j::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            j::make_heap(v.begin(), v.end());
            for (size_t i = 0; i < N; ++i) {
                j::pop_heap(v.begin(), v.end() - i);
            }
            return v.size();
        };

        BENCHMARK("std::pop_heap with j::deque") {
            j::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::make_heap(v.begin(), v.end());
            for (size_t i = 0; i < N; ++i) {
                std::pop_heap(v.begin(), v.end() - i);
            }
            return v.size();
        };
    }

    SECTION("std::deque containers") {
        BENCHMARK("j::pop_heap with std::deque") {
            std::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            j::make_heap(v.begin(), v.end());
            for (size_t i = 0; i < N; ++i) {
                j::pop_heap(v.begin(), v.end() - i);
            }
            return v.size();
        };

        BENCHMARK("std::pop_heap with std::deque") {
            std::deque<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::make_heap(v.begin(), v.end());
            for (size_t i = 0; i < N; ++i) {
                std::pop_heap(v.begin(), v.end() - i);
            }
            return v.size();
        };
    }
}

TEST_CASE("Priority Queue Benchmarks: push/pop operations") {
    SECTION("j::priority_queue vs std::priority_queue with std::vector") {
        BENCHMARK("j::priority_queue push/pop") {
            j::priority_queue<int, std::vector<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.push(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };

        BENCHMARK("std::priority_queue push/pop") {
            std::priority_queue<int, std::vector<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.push(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };
    }

    SECTION("j::priority_queue with different containers") {
        BENCHMARK("j::priority_queue with j::vector") {
            j::priority_queue<int, j::vector<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.push(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };

        BENCHMARK("j::priority_queue with std::vector") {
            j::priority_queue<int, std::vector<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.push(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };
    }

    SECTION("std::priority_queue with different containers") {
        BENCHMARK("std::priority_queue with j::vector") {
            std::priority_queue<int, j::vector<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.push(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };

        BENCHMARK("std::priority_queue with std::vector") {
            std::priority_queue<int, std::vector<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.push(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };
    }
}

TEST_CASE("Priority Queue Benchmarks: emplace operations") {
    SECTION("j::priority_queue vs std::priority_queue emplace") {
        BENCHMARK("j::priority_queue emplace") {
            j::priority_queue<int, std::vector<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.emplace(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };

        BENCHMARK("std::priority_queue emplace") {
            std::priority_queue<int, std::vector<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.emplace(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };
    }
}

TEST_CASE("Heap Algorithm Benchmarks: sort_heap") {
    SECTION("Heap sort comparison") {
        BENCHMARK("j::heap_sort simulation") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            j::make_heap(v.begin(), v.end());
            for (size_t i = 0; i < N; ++i) {
                j::pop_heap(v.begin(), v.end() - i);
            }
            return v.size();
        };

        BENCHMARK("std::heap_sort simulation") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::make_heap(v.begin(), v.end());
            for (size_t i = 0; i < N; ++i) {
                std::pop_heap(v.begin(), v.end() - i);
            }
            return v.size();
        };
    }
}

TEST_CASE("Heap Algorithm Benchmarks: custom comparator") {
    SECTION("Min heap operations") {
        BENCHMARK("j::heap algorithms with greater") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            j::make_heap(v.begin(), v.end(), std::greater<int>());
            for (size_t i = 0; i < N / 2; ++i) {
                j::pop_heap(v.begin(), v.end() - i, std::greater<int>());
            }
            return v.size();
        };

        BENCHMARK("std::heap algorithms with greater") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::make_heap(v.begin(), v.end(), std::greater<int>());
            for (size_t i = 0; i < N / 2; ++i) {
                std::pop_heap(v.begin(), v.end() - i, std::greater<int>());
            }
            return v.size();
        };
    }
}

TEST_CASE("Priority Queue Benchmarks: Different Orders") {
    SECTION("Max heap (default)") {
        BENCHMARK("j::priority_queue max heap") {
            j::priority_queue<int, std::vector<int>, std::less<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.push(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };

        BENCHMARK("std::priority_queue max heap") {
            std::priority_queue<int, std::vector<int>, std::less<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.push(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };
    }

    SECTION("Min heap") {
        BENCHMARK("j::priority_queue min heap") {
            j::priority_queue<int, std::vector<int>, std::greater<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.push(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };

        BENCHMARK("std::priority_queue min heap") {
            std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
            for (size_t i = 0; i < N; ++i) {
                pq.push(dis(gen));
            }
            size_t sum = 0;
            while (!pq.empty()) {
                sum += pq.top();
                pq.pop();
            }
            return sum;
        };
    }
}
